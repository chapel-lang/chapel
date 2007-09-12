#ifndef _stringutil_H_
#define _stringutil_H_

char *dupstr(const char *s, const char *e = 0);
int stringlen(const char* s1);
int stringlen(const char* s1, const char* s2, const char* s3=NULL, 
              const char* s4 = NULL, const char* s5 = NULL);


char* stringcpy(const char* s1);
char* stringcat(const char* s1, const char* s2 = "", const char* s3 = "", 
                const char* s4 = "", const char* s5 = "");
char* stringinsert(const char* origstr, const char* insertpt, const char* newstr);

char* intstring(int i);

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
