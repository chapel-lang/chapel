#include <stdio.h>

static void print_c_string(const char* arg)
{
  printf("in print_c_string: %s\n", arg);
}

static void print_c_ptrConst(const char* arg)
{
  printf("in print_c_ptrConst: %s\n", arg);
}
