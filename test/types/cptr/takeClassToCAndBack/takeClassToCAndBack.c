#include "stdio.h" // printf

#include "takeClassToCAndBack.h" // functions for working with Chapel's Foo class

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);

  void* fooInstance; // class Foo in Chapel, but we only use a void* to it
  getInstance(&fooInstance, 4);
  printf("%i\n", useInstance(fooInstance));
  destroy_func(&fooInstance);

  chpl_library_finalize();
}
