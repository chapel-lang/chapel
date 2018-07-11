class C {
  type t;
  var f : t;
}

var c = new borrowed C(f = 1);

writeln(c);
