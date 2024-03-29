#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#ifdef __cplusplus

static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

#endif
