module OuterModule {
  // Tests basic external variables and inline functions
  // defined both within the extern "C" module and in an
  // included C header file. Also tests inclusion
  // of C standard library headers and variadic functions.
  module C { extern {
    #include "basic.h"
    #include <stdbool.h>

    static inline void print_bool(bool x) { if (x) printf("yes\n"); else printf("no\n"); }
    
    static inline int add_two(int x, int y) { return x+y; }
    
    double my_doub = 3.14;
    const char* greet_str = "Hello";
  } }

  try {
    writeln(createStringWithNewBuffer(C.greeting()));
  }
  catch e: DecodeError {
    writeln("Decode error creating string");
  }
  catch {
    writeln("Unknown error creating string");
  }
  writeln(C.my_doub);
  writeln(C.my_int);
  writeln(C.add_one(1000));

  writeln(C.add_two(17, 12));

  C.print_bool(true);
  C.print_bool(false);

  //NOTE: Chapel's non-support of zero-length
  //  tuples means var args must always define
  //  at least one argument. I.e., printf must
  //  include at least one argument in addition
  //  to the format string.
  C.printf("%s, world!\n".c_str(), C.greet_str);
}
