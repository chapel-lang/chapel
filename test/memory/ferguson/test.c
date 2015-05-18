/*#undef _POSIX_C_SOURCE
//#ifndef _BSD_SOURCE
#define _BSD_SOURCE
//#endif
//#ifndef _GNU_SOURCE
#define _GNU_SOURCE
//#endif
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

uint64_t* c_function_malloc(void)
{
  uint64_t* ret;
  printf("enter c_function_malloc\n");
  ret = (uint64_t*) malloc(16);
  printf("exit c_function_malloc\n");
  return ret;
}
  
uint64_t* c_function_calloc(void)
{
  uint64_t* ret;
  printf("enter c_function_calloc\n");
  ret = (uint64_t*) calloc(16,1);
  printf("exit c_function_calloc\n");
  return ret;
}
 

uint64_t* c_function_strdup(void)
{
  char* ret;
  printf("enter c_function_strdup\n");
  ret = strdup("This is just a test"); //needs to be >= 8b long
  printf("exit c_function_strdup\n");
  return (uint64_t*) ret;
}

void c_function_free(uint64_t* arg)
{
  printf("enter c_function_free\n");
  free(arg);
  printf("exit c_function_free\n");
}



