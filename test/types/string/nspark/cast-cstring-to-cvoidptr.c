#include "cast-cstring-to-cvoidptr.h"

size_t strlen_voidptr(const void *s)
{
  return strlen((const char*)s);
}
