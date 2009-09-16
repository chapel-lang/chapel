class C {
  type t;
  param p: int = 1;
  var x: t;
}

var c: C(int);

c = new C(int);

writeln(c);

delete c;
