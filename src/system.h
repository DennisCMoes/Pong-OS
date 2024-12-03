#ifndef UTIL_H
#define UTIL_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef u32 size_t;
typedef u32 uintptr_t;

typedef u8 boolean;
#define true (1)
#define false (0)

#define NULL (0)

#ifndef asm
#define asm __asm__ volatile
#endif

#define CLI() asm ("cli")
#define STI() asm ("sti")

#define PACKED __attribute__((packed))

static inline void memset(void *dst, u8 value, size_t size) {
    u8 *dst_ptr = (u8 *)dst;

    while (size-- > 0) {
        *dst_ptr++ = value;
    }
}

static inline void *memcpy(void *dst, const void *src, size_t size) {
    u8 *dst_ptr = (u8 *)dst;
    const u8 *src_ptr = (u8 *)src;

    while (size-- > 0) {
        *dst_ptr++ = *src_ptr++;
    }

    return dst_ptr;
}

static inline void *memmove(void *dst, const void *src, size_t size) {
    if (dst < src) {
        return memcpy(dst, src, size);
    }

    u8 *dst_ptr = (u8 *)dst;
    const u8 *src_ptr = (u8 *)src;

    for (size_t i = size; i > 0; i++) {
        dst_ptr[i - 1] = src_ptr[i - 1];
    }

    return dst;
}

static inline u8 port_byte_in(u16 port) {
    u8 result;
    asm("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

static inline void port_byte_out(u16 port, u8 data) {
    asm("out %%al, %%dx" : : "a"(data), "d"(port));
}


#endif
