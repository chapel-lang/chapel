#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "stringutil.h"

static ChainHashMap<const char*, StringHashFns, const char*> chapelStringsTable;

static const char*
canonicalize_string(const char *s) {
  const char* ss = chapelStringsTable.get(s);
  if (!ss) {
    chapelStringsTable.put(s, s);
    return s;
  }
  return ss;
}

const char*
astr(const char* s1, const char* s2, const char* s3, const char* s4) {
  int len;
  len = strlen(s1);
  if (s2)
    len += strlen(s2);
  if (s3)
    len += strlen(s3);
  if (s4)
    len += strlen(s4);
  char* s = (char*)malloc(len+1);
  strcpy(s, s1);
  if (s2)
    strcat(s, s2);
  if (s3)
    strcat(s, s3);
  if (s4)
    strcat(s, s4);
  const char* t = canonicalize_string(s);
  if (s != t)
    free(s);
  return t;
}

const char*
istr(int i) {
  char s[64];
  if (sprintf(s, "%d", i) > 63)
    INT_FATAL("istr buffer overflow");
  return astr(s);
}

//
// returns a canonicalized substring that contains the first part of
// 's' up to 'e'
// note: e must be in s
//
const char* asubstr(const char* s, const char* e) {
  char* ss = (char*)malloc(e-s+1);
  strncpy(ss, s, e-s);
  ss[e-s] = '\0';
  const char* t = canonicalize_string(ss);
  if (ss != t)
    free(ss);
  return t;
}


void deleteStrings() {
  Vec<const char*> keys;
  chapelStringsTable.get_keys(keys);
  forv_Vec(const char, key, keys) {
    free((void*)key);
  }
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
