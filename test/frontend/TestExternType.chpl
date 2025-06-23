use Print;
use CTypes;

extern {
  typedef struct a_ { int x; } a;

  typedef struct b_ {
    int x;
    double y;
  } b;
}

extern type a;

extern record b {
  var x: c_int;
  var y: c_double;
}

proc main() {
  var v1: a;
  var v2: b;
  println(v2.x); // should be zeroed out

  // test assignment between extern types
  var x : b;
  var y : b;

  // initialize some values
  // (use 'c_int' as a workaround for bug where coercion doesn't work yet)
  x.x = 5:c_int;
  y.x = 42:c_int;

  println(x.x);

  x = y;

  println(x.x);
}
