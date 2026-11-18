module C {
  use CTypes;
  extern {
    #define MODULE_ADD(a, b) ((a) + (b))
  }
}

use CTypes;
use C only MODULE_ADD;
use C only MODULE_ADD as RENAMED_ADD;
import C.MODULE_ADD as IMPORTED_ADD;

assert(MODULE_ADD(2:c_int, 3:c_int) == 5);
assert(RENAMED_ADD(3:c_int, 4:c_int) == 7);
assert(IMPORTED_ADD(4:c_int, 5:c_int) == 9);
assert(C.MODULE_ADD(2:c_int, 0.5:c_double) == 2.5);
writeln("module macro imports passed");
