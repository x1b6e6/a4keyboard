#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned int u32;

struct write_args {
    u8 data[64];
    u32 hid;
};

#ifdef __cplusplus
}
#endif
