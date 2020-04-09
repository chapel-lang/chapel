#include <stdio.h>

extern void chpl_library_init(int argc, char* argv[]);
extern void chpl_library_finalize(void);

extern void foo(void);

int main(int argc, char* argv[]) {
  printf("In C main()\n");
  chpl_library_init(argc, argv);
  foo();
  chpl_library_finalize();
  return 0;
}
