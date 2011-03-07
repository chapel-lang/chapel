// The type bindings below ('C.' etc.) will ideally become legal.
// For now we are testing the code in the compiler to generate errors.

class C {
  proc C.cc() {}
}

var c = new C();
writeln(c);

record R {
  proc R.rr() {}
}

var r: R;
writeln(c);

union U {
  var i:int; var r:real;
  proc U.uu() {}
}

var u: U;
writeln(u);
