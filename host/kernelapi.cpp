#include "kernelapi.h"

#include <bpf/bpf.h>

#include "bpf.api.h"
#include "kernel.skel.h"

KernelApi::KernelApi(struct kernel_bpf_c *obj)
    : m_kernelObject(obj)
{
}

KernelApi::KernelApi(KernelApi &&other)
{
    std::swap(m_kernelObject, other.m_kernelObject);
}

KernelApi::~KernelApi()
{
    if (m_kernelObject)
        kernel_bpf_c__destroy(m_kernelObject);
    m_kernelObject = nullptr;
}

std::optional<KernelApi> KernelApi::create()
{
    struct kernel_bpf_c *obj = kernel_bpf_c__open_and_load();
    if (obj)
        return KernelApi{obj};
    return std::nullopt;
}

int KernelApi::write(int hid, const uint8_t (&data)[64])
{
    struct write_args args;

    static_assert(sizeof(args.data) == sizeof(data));

    __builtin_memcpy(args.data, data, sizeof(data));
    args.hid = hid;

    DECLARE_LIBBPF_OPTS(bpf_test_run_opts, tattrs, .ctx_in = &args, .ctx_size_in = sizeof(args), );
    int prog_fd = bpf_program__fd(m_kernelObject->progs.write);
    int ret = bpf_prog_test_run_opts(prog_fd, &tattrs);

    return ret > 0 ? 0 : ret;
}
