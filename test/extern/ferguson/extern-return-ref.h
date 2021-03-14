#include <stdio.h>

static inline int64_t* extern_ref_identity(int64_t* arg)
{
  printf("extern_ref_identity\n");
  return arg;
}
static inline const int64_t* extern_const_ref_identity(const int64_t* arg)
{
  printf("extern_const_ref_identity\n");
  return arg;
}
