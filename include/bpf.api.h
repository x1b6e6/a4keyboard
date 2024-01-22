#pragma once

typedef unsigned char u8;
typedef unsigned int u32;

struct write_args {
    u8 data[64];
    u32 hid;
};
typedef struct write_args write_args_t;

struct command_args {
    u8 data[16];
    u32 hid;
};
typedef struct command_args command_args_t;
