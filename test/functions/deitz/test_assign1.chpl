record C {
  var i : int = 2;
}

record D {
  var j : int = 3;
}

proc D.init=(other : D) {
  this.j = other.j;
}
proc =(ref lhs : D, const ref rhs : D) {
  lhs.j = rhs.j;
}

proc =(ref d : D, c : C) {
  d.j = c.i;
}

var c : C;
var d : D;

writeln(c);
writeln(d);

d = c;

writeln(d);
