use test1;

extern {
  #include "test2.h"
}

proc main() {
  var x = makeX(); // makeX, X come from test1
  useX(x); // useX comes from test2
}
