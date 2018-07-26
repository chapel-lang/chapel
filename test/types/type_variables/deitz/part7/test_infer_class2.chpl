class C {
  type t;
  var f : t;
}

var c = new borrowed C(f = 1);

writeln(c);

var d : borrowed C(int) = new borrowed C(int, 12);

writeln(d);
