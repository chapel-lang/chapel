#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "chplalloc.h"
#include "misc.h"
#include "stringutil.h"


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
  char* s = (char*)MALLOC(stringlen(s1) + 1);
  strcpy(s, s1);
  return s;
}


char* stringcat(const char* s1, const char* s2, const char* s3, const char* s4,
                const char* s5) {
  char* s = (char*)MALLOC(stringlen(s1, s2, s3, s4, s5) + 1);
  sprintf(s, "%s%s%s%s%s", s1, s2, s3, s4, s5);
  return s;
}


char* intstring(int i) {
  char buf[64];
  if (sprintf(buf, "%d", i) > 63)
    INT_FATAL("intstring() buffer overflow");
  return stringcpy(buf);
}
