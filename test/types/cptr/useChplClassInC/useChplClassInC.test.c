#include "stdio.h" // printf

// Required functions for using Chapel library code
extern void chpl_library_init(int argc, char* argv[]);
extern void chpl_library_finalize(void);

// Chapel functions for working with Foo opaquely
extern void getInstance(void*, int);
extern int useInstance(void*);
extern void deleteInstance(void*);

// C main that uses the exported Chapel functions
int main(int argc, char** argv) {
  chpl_library_init(argc, argv);

  void* fooInstance; // class Foo in Chapel, but we only use a void* to it
  getInstance(&fooInstance, 4);
  printf("%i\n", useInstance(fooInstance));
  deleteInstance(fooInstance);

  chpl_library_finalize();
}
