class C {
  def C.cc() {}
}

record R {
  def R.rr() {}
}

union U {
  var i:int; var r:real;
  def U.uu() {}
}

var c = new C();
writeln(c);

var r: R;
writeln(r);

var u: U; u.i = 5;
writeln(u);
