#ifndef _HELPERS_H_
#define _HELPERS_H_


#include <cinttypes>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <string.h>



typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;



bool startsWith(const char* str, const char* prefix);
void clean_exit(int status);
char* findProgramPath(const char *argv0);
void setupError(std::string msg);

extern bool developer;

#define INT_FATAL(MSG)     setupError(MSG)

#define USR_FATAL(MSG)     setupError(MSG)

#define USR_FATAL_CONT(MSG) setupError(MSG)


uint64_t    hexStr2uint64(const char* str, bool userSupplied = false,
                          const char* filename = NULL, int line = -1);
uint64_t    str2uint64(const char* str, bool userSupplied = false,
                       const char* file = NULL, int line = -1);
int64_t     str2int64(const char* str, bool userSupplied = false,
                      const char* file = NULL, int line = -1);


#endif
