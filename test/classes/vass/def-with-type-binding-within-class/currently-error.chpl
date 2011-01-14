// The type bindings below ('C.' etc.) will ideally become legal.
// For now we are testing the code in the compiler to generate errors.

class C {
  def C.cc() {}
}

var c = new C();
writeln(c);

record R {
  def R.rr() {}
}

var r: R;
writeln(c);

union U {
  var i:int; var r:real;
  def U.uu() {}
}

var u: U;
writeln(u);
