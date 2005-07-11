class C {
  type t;
  var x : t;
}

var c : C(t = integer) = C(t = integer);
c.x = 12;
writeln(c);
