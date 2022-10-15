
#ifndef VEC_H
#define VEC_H

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

typedef struct __attribute__((__packed__)) Vec {
	u16 len;
	char data[];
} vector;



#endif

