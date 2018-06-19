class C {
  type t;
  var x: t;
}

var c = borrowed C(int, 1);
writeln(c);
