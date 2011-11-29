class C {
  type t;
  var x : t;
  var n : C(t);
}

var c = new C(int);

writeln(c);

delete c;
