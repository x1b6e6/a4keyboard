#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct write_args {
    u8 data[64];
    u32 hid;
};

#ifdef __cplusplus
}
#endif
