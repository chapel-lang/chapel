/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _stringutil_H_
#define _stringutil_H_

const char* astr(const char* s1, const char* s2 = NULL,
                 const char* s3 = NULL, const char* s4 = NULL,
                 const char* s5 = NULL, const char* s6 = NULL,
                 const char* s7 = NULL, const char* s8 = NULL);
const char* istr(int i);
const char* asubstr(const char* s, const char* e);

void deleteStrings();

int8_t  str2int8(const char* str);
int16_t  str2int16(const char* str);
int32_t  str2int32(const char* str);
int64_t  str2int64(const char* str);
uint8_t str2uint8(const char* str);
uint16_t str2uint16(const char* str);
uint32_t str2uint32(const char* str);
uint64_t str2uint64(const char* str);

uint64_t binStr2uint64(const char* str);
uint64_t hexStr2uint64(const char* str);

#endif
