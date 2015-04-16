#include <inttypes.h>
#include <stdio.h>


static int64_t* getaddress(void)
{
  static int64_t ret;
  return &ret;
}

static int64_t* getaddress2(void)
{
  static int64_t ret;
  return &ret;
}


