record C {
  var i : int = 2;
}

record D {
  var j : int = 3;
}

proc =(d : D, c : C) {
  d.j = c.i;
  return d;
}

var c : C;
var d : D;

writeln(c);
writeln(d);

d = c;

writeln(d);
