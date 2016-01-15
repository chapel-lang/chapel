#include <stdio.h>
extern char** environ;

int main(int argc, char** argv)
{
  int i;
  for( i = 0; i < argc; i++ ) {
    printf("%s\n", argv[i]);
  }
  for( i = 0; environ[i]; i++ ) {
    printf("%s\n", environ[i]);
  }
  printf("OK\n");
  return 0;
}

