#include <stdio.h>

static const char* returns_c_string(void) { return "returned_c_string"; }
static void returns_c_string_in_argument(const char** arg)
{
  *arg = "returned_c_string_in_argument";
}

static void print_c_string(const char* arg)
{
  printf("in print_c_string: %s\n", arg);
}

static void print_c_string_len(const char* arg, int len)
{
  printf("in print_c_string_len: %.*s\n", len, arg);
}


static void return_c_string_in_argument_with_length(const char** arg, int* len)
{
  *arg = "returned_c_string_in_argument_with_length BUT NOT THIS PART";
  *len = 38;
}

