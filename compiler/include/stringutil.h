#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <stdint.h>

const char* astr(const char* s1,
                 const char* s2 = 0,
                 const char* s3 = 0,
                 const char* s4 = 0,
                 const char* s5 = 0,
                 const char* s6 = 0,
                 const char* s7 = 0,
                 const char* s8 = 0);

const char* istr(int i);

const char* asubstr(const char* s, const char* e);

void        deleteStrings();

int8_t      str2int8(const char* str);
int16_t     str2int16(const char* str);
int32_t     str2int32(const char* str);
int64_t     str2int64(const char* str);
uint8_t     str2uint8(const char* str);
uint16_t    str2uint16(const char* str);
uint32_t    str2uint32(const char* str);
uint64_t    str2uint64(const char* str);

uint64_t    binStr2uint64(const char* str);
uint64_t    hexStr2uint64(const char* str);

#endif
