class C {
  type t;
  var x : t;
}

var c : C(t = int) = C(t = int);
c.x = 12;
writeln(c);
