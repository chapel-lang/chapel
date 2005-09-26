#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "chplalloc.h"
#include "misc.h"
#include "stringutil.h"

int stringlen(const char* s1) {
  return strlen(s1);
}

int stringlen(const char* s1, const char* s2) {
  return strlen(s1) + strlen(s2);
}

int stringlen(const char* s1, const char* s2, const char* s3) {
  return stringlen(s1, s2) + strlen(s3);
}

int stringlen(const char* s1, const char* s2, const char* s3, const char* s4) {
  return stringlen(s1, s2, s3) + strlen(s4);
}

int stringlen(const char* s1, const char* s2, const char* s3, const char* s4, const char* s5) {
  return stringlen(s1, s2, s3, s4) + strlen(s5);
}

char* stringcpy(const char* s1) {
  char* s = (char*)MALLOC(stringlen(s1) + 1);
  sprintf(s, "%s", s1);
  return s;
}

char* stringcat(const char* s1, const char* s2) {
  char* s = (char*)MALLOC(stringlen(s1, s2) + 1);
  sprintf(s, "%s%s", s1, s2);
  return s;
}

char* stringcat(const char* s1, const char* s2, const char* s3) {
  char* s = (char*)MALLOC(stringlen(s1, s2, s3) + 1);
  sprintf(s, "%s%s%s", s1, s2, s3);
  return s;
}

char* stringcat(const char* s1, const char* s2, const char* s3, const char* s4) {
  char* s = (char*)MALLOC(stringlen(s1, s2, s3, s4) + 1);
  sprintf(s, "%s%s%s%s", s1, s2, s3, s4);
  return s;
}

char* stringcat(const char* s1, const char* s2, const char* s3, const char* s4, const char* s5) {
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
