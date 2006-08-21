class C {
  type t;
  var f : t;
}

var c = C(f = 1);

writeln(c);

var d : C(int) = C(int, 12);

writeln(d);
