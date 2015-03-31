#include <inttypes.h>
#include <stdio.h>


static int64_t* getaddress(void)
{
  static int64_t ret;
  return &ret;
}

 
