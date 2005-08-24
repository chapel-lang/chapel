class C {
  type t;
  var f : t;
}

var c = C(f = 1);

writeln(c);

var d : C(integer) = C(integer, 12);

writeln(d);
