#ifndef __LCT_COMMON_DEFINE_H__
#define __LCT_COMMON_DEFINE_H__

#include <cstdint>
#include <cstddef>
#include <string>

typedef unsigned char			lct_uchar_t;
typedef char					lct_char_t;
typedef intmax_t				lct_intmax_t;
typedef uintmax_t				lct_uintmax_t;
typedef int8_t					lct_int8_t;
typedef uint8_t					lct_uint8_t;
typedef int16_t					lct_int16_t;
typedef uint16_t				lct_uint16_t;
typedef int32_t					lct_int32_t;
typedef uint32_t				lct_uint32_t;
typedef int64_t					lct_int64_t;
typedef uint64_t				lct_uint64_t;
typedef size_t					lct_size_t;

typedef std::string				lct_string_t;

typedef bool					lct_bool_t;

#include "lct_error_code.h"
#include "lct_trace_log.h"

#define RPS_ROOM_NAME_MAX_LENGTH	24

#endif
