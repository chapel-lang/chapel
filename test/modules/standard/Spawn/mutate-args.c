#include <stdio.h>
extern char** environ;

int main(int argc, char** argv)
{
  int i;
  for( i = 0; i < argc; i++ ) argv[i][0] = '\0';
  for( i = 0; environ[i]; i++ ) environ[i][0] = '\0';
  printf("OK\n");
  return 0;
}

