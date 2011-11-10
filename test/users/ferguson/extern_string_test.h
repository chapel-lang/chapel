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

