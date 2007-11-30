#ifndef _stringutil_H_
#define _stringutil_H_

const char* astr(const char* s1, const char* s2 = NULL,
                 const char* s3 = NULL, const char* s4 = NULL);
const char* istr(int i);
const char* asubstr(const char* s, const char* e);

void deleteStrings();

int8  str2int8(const char* str);
int16  str2int16(const char* str);
int32  str2int32(const char* str);
int64  str2int64(const char* str);
uint8 str2uint8(const char* str);
uint16 str2uint16(const char* str);
uint32 str2uint32(const char* str);
uint64 str2uint64(const char* str);

uint64 binStr2uint64(const char* str);
uint64 hexStr2uint64(const char* str);

#endif
