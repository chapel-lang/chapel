#include <string.h>
  
static
void get_string(const char** string_out)
{
  *string_out = strdup("Hello");
  printf("get_string returns %s\n", *string_out);
}

static
void modify_string(const char** string_out, const char* string_in)
{
  char* tmp;
  printf("modify_string got %s\n", string_in);
  tmp = strdup(string_in);
  tmp[0] = 'A';
  *string_out = tmp;
  printf("modify_string returns %s\n", *string_out);
}

