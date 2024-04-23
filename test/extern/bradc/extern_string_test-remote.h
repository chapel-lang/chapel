#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <string.h>

static inline
const char* return_string_test(void)
{
  return strdup("return string test");
}

static inline
void return_string_arg_test(const char** str)
{
  *str = strdup("return string arg test");
}

static inline
const char* return_c_ptrConstUchar_test(void)
{
  return strdup("return c_ptrConst(c_uchar) test");
}

static inline
void return_c_ptrConstUchar_arg_test(const char** str)
{
  *str = strdup("return c_ptrConst(c_uchar) arg test");
}

static inline
const char* return_c_ptrConstChar_test(void)
{
  return strdup("return c_ptrConst(c_char) test");
}

static inline
void return_c_ptrConstChar_arg_test(const char** str)
{
  *str = strdup("return c_ptrConst(c_char) arg test");
}

static inline
char* return_c_ptrUchar_test(void)
{
  return strdup("return c_ptr(c_uchar) test");
}

static inline
void return_c_ptrUchar_arg_test(const char** str)
{
  *str = strdup("return c_ptr(c_uchar) arg test");
}

static inline
char* return_c_ptrChar_test(void)
{
  return strdup("return c_ptr(c_char) test");
}

static inline
void return_c_ptrChar_arg_test(const char** str)
{
  *str = strdup("return c_ptr(c_char) arg test");
}
