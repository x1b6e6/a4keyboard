#pragma once

#include <cstdint>
#include <optional>

struct kernel_bpf_c;

class KernelApi
{
public:
    KernelApi(struct kernel_bpf_c *);
    KernelApi(const KernelApi &) = delete;
    KernelApi(KernelApi &&other);
    ~KernelApi();

    int write(int hid, const uint8_t (&data)[64]);

    static std::optional<KernelApi> create();

private:
    struct kernel_bpf_c *m_kernelObject{};
};
