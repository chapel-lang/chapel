class C {
  type t;
  var x: t;
}

var c = new C([1..3] int);
c.x[2] = 4;
writeln(c);
