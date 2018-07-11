class C {
  type t;
  var x : t;
}

var c : borrowed C(t = int) = new borrowed C(t = int);
c.x = 12;
writeln(c);
