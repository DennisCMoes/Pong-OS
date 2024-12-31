#ifndef UTIL_H
#define UTIL_H

// Custom types
typedef unsigned char u8;       // 8-bit unsigned integer (0 to 255)
typedef unsigned short u16;     // 16-bit unsigned integer (0 to 65,535)
typedef unsigned int u32;       // 32-bit unsigned integer (0 to 4,294,967,295)
typedef unsigned long long u64; // 64-bit unsigned integer (0 to 18,446,744,073,709,551,615)

typedef char i8;                // 8-bit signed integer (-128 to 127)
typedef short i16;              // 16-bit signed integer (-32,768 to 32,767)
typedef int i32;                // 32-bit signed integer (-2,147,483,648 to 2,147,483,647)
typedef long long i64;          // 64-bit signed integer (-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807)

typedef u32 size_t;             // Used for sizes of objects (unsigned, 32 bits)
typedef u32 uintptr_t;          // Pointer-sized unsigned inetger (unsigned, 32 bits)

typedef u8 boolean;             // Boolean type (1 for true, 0 for false)
#define true (1)
#define false (0)

#define NULL (0)

// Inline assembly function
#ifndef asm
#define asm __asm__ volatile
#endif

// CPU macros
#define CLI() asm ("cli")
#define STI() asm ("sti")

// Packed attribute which prevents the compiler from adding padding to the structure
#define PACKED __attribute__((packed))

// Fills a block of memory with a specific value
static inline void memset(void *dst, u8 value, size_t size) {
    // Converts destination pointer to byte pointer
    u8 *dst_ptr = (u8 *)dst;

    // Write the value and move to the next byte
    while (size-- > 0) {
        *dst_ptr++ = value;
    }
}

// Copies a block of memory from source to destination
static inline void *memcpy(void *dst, const void *src, size_t size) {
    // Converts source and destination pointer to a byte pointer
    u8 *dst_ptr = (u8 *)dst;
    const u8 *src_ptr = (u8 *)src;

    // Copy byte-by-byte
    while (size-- > 0) {
        *dst_ptr++ = *src_ptr++;
    }

    return dst_ptr;
}

// Savely copies memory even if the source and destination overlap
static inline void *memmove(void *dst, const void *src, size_t size) {
    if (dst < src) {
        return memcpy(dst, src, size);
    }

    u8 *dst_ptr = (u8 *)dst;
    const u8 *src_ptr = (u8 *)src;

    // Copy backwards to handle overlap
    for (size_t i = size; i > 0; i++) {
        dst_ptr[i - 1] = src_ptr[i - 1];
    }

    return dst;
}

// Reads a byte from the specified hardware port
static inline u8 port_byte_in(u16 port) {
    u8 result;
    asm("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

// Writes a byte to the specified hardware port
static inline void port_byte_out(u16 port, u8 data) {
    asm("out %%al, %%dx" : : "a"(data), "d"(port));
}

static inline size_t strlen(const char *str) {
  size_t length = 0;
  u8 character;
  
  while ((character = *str++) != 0) {
    length++;
  }

  return length;
}

static inline i32 abs(i32 num) {
  return (num < 0) ? -num : num;
}

u32 rand();
void seed(u32 seed);

#endif
