#include "lib/interopWithC.h"

int main(int argc, char* argv[]) {
  // initialize the library's copy of the Chapel runtime
  chpl_library_init(argc, argv);

  // execute the module initialization (module-level code) for
  // 'interopWithC', including that of any modules it uses or imports
  chpl__init_interopWithC(1, 2);

  // call its exported routines
  foo();
  alsoCallsBaz();
  callUseMyType();

  chpl_library_finalize();
}
