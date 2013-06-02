extern {
#include "shadow.h"
}

writeln(foo(1));
proc foo(x) {
  return x + 2;
}

writeln(foo(1));
