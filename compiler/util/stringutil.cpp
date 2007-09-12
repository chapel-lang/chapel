#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "stringutil.h"


char *
dupstr(const char *s, const char *e) {
  int l = e ? e-s : strlen(s);
  char *ss = (char*)malloc(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}



int stringlen(const char* s1) {
  if (s1 == NULL) {
    return 0;
  } else {
    return strlen(s1);
  }
}


int stringlen(const char* s1, const char* s2, const char* s3, const char* s4,
              const char* s5) {
  return stringlen(s1) + stringlen(s2) + stringlen(s3) + stringlen(s4) + 
    stringlen(s5);
}


char* stringcpy(const char* s1) {
  char* s = (char*)malloc(stringlen(s1) + 1);
  strcpy(s, s1);
  return s;
}


char* stringcat(const char* s1, const char* s2, const char* s3, const char* s4,
                const char* s5) {
  char* s = (char*)malloc(stringlen(s1, s2, s3, s4, s5) + 1);
  sprintf(s, "%s%s%s%s%s", s1, s2, s3, s4, s5);
  return s;
}


char* stringinsert(const char* sorig, const char* pos, const char* insertme) {
  return stringcat(dupstr(sorig, pos), insertme, dupstr(pos+1));
}


char* intstring(int i) {
  char buf[64];
  if (sprintf(buf, "%d", i) > 63)
    INT_FATAL("intstring() buffer overflow");
  return stringcpy(buf);
}


#define define_str2Int(type, format)                            \
  type str2##type(const char* str) {                            \
    if (!str) {                                                 \
      INT_FATAL("NULL string passed to strTo_" #type "()");     \
    }                                                           \
    int len = strlen(str);                                      \
    if (len < 1) {                                              \
      INT_FATAL("empty string passed to strTo_" #type "()");    \
    }                                                           \
    type val;                                                   \
    int numitems = sscanf(str, format, &val);                   \
    if (numitems != 1) {                                        \
      INT_FATAL("Illegal string passed to strTo_" #type "()");  \
    }                                                           \
    return val;                                                 \
  }

define_str2Int(int8, int8fmt)
define_str2Int(int16, int16fmt)
define_str2Int(int32, int32fmt)
define_str2Int(int64, int64fmt)
define_str2Int(uint8, uint8fmt)
define_str2Int(uint16, uint16fmt)
define_str2Int(uint32, uint32fmt)
define_str2Int(uint64, uint64fmt)


uint64 binStr2uint64(const char* str) {
  if (!str) {
    INT_FATAL("NULL string passed to binStrToUint64()");
  }
  int len = strlen(str);
  if (len < 3 || str[0] != '0' || str[1] != 'b') {
    INT_FATAL("Illegal string passed to binStrToUint64()");
  }
  uint64 val = 0;
  for (int i=2; i<len; i++) {
    val <<= 1;
    switch (str[i]) {
    case '0':
      break;
    case '1':
      val += 1;
      break;
    default:
      INT_FATAL("illegal character in binary string: '%c'", str[i]);
    }
  }
  return val;
}


uint64 hexStr2uint64(const char* str) {
  if (!str) {
    INT_FATAL("NULL string passed to binStrToUint64()");
  }
  int len = strlen(str);
  if (len < 3 || str[0] != '0' || str[1] != 'x') {
    INT_FATAL("Illegal string passed to binStrToUint64()");
  }
  uint64 val;
  int numitems = sscanf(str+2, "%llx", &val);
  if (numitems != 1) {
    INT_FATAL("Illegal string passed to hexStrToUint64");
  }
  return val;
}
