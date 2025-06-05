use CTypes;

extern {
  typedef struct a_ { int x; } a;
}

extern type a;

extern record b {
  var x: c_int;
  var y: c_double;
}

proc main() {
  var v1: a;
  var v2: b;
}
