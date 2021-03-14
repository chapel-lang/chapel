use common;

extern {
  #include "test.h"
}

var x : MATRIX_type;

x.matrix = 27.0;
f(1.0, getPtr(x));
writeln(x);
