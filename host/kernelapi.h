#pragma once

#include <cstdint>
#include <optional>

#include "types.h"

struct kernel_bpf_c;

class KernelApi
{
public:
    KernelApi(struct kernel_bpf_c *);
    KernelApi(const KernelApi &) = delete;
    KernelApi(KernelApi &&other);
    ~KernelApi();

    int write(u32 hid, const u8 (&data)[64]);

    static std::optional<KernelApi> create();

private:
    struct kernel_bpf_c *m_kernelObject{};
};
