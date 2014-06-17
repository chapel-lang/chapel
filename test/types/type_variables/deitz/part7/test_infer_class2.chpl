class C {
  type t;
  var f : t;
}

var c = new C(f = 1);

writeln(c);

var d : C(int) = new C(int, 12);

writeln(d);
