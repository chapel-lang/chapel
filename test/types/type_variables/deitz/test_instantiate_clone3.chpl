class C {
  type t;
  var x : t;
  var y;
}

var c = C(t=int, y=14.0);
c.y = 12;
writeln(c);
