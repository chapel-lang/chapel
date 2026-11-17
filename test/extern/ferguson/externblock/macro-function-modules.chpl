module C {
  use CTypes;
  extern {
    #define MODULE_ADD(a, b) ((a) + (b))
  }
}

use CTypes;
use C only MODULE_ADD;

assert(MODULE_ADD(2:c_int, 3:c_int) == 5);
assert(C.MODULE_ADD(2:c_int, 0.5:c_double) == 2.5);
writeln("module macro imports passed");
