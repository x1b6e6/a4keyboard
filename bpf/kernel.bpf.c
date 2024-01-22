#include "vmlinux.h"

#include <bpf/bpf_helpers.h>
#include <errno.h>

#include "bpf.api.h"

extern struct hid_bpf_ctx *hid_bpf_allocate_context(unsigned int hid_id) __ksym;
extern void hid_bpf_release_context(struct hid_bpf_ctx *ctx) __ksym;
extern int hid_bpf_hw_request(struct hid_bpf_ctx *ctx, __u8 *data, size_t len,
        enum hid_report_type type, enum hid_class_request reqtype) __ksym;

SEC("syscall")
int write(write_args_t *arg)
{
    struct hid_bpf_ctx *ctx = hid_bpf_allocate_context(arg->hid);
    if (!ctx)
        return -EPERM;

    int ret = hid_bpf_hw_request(
            ctx, arg->data, sizeof(arg->data), HID_FEATURE_REPORT, HID_REQ_SET_REPORT);
    hid_bpf_release_context(ctx);

    return ret;
}

char _license[] SEC("license") = "GPL";
