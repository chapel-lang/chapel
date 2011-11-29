record point {
  var x : int;
  var y : int;
}

proc +(p1 : point, p2 : point) : point {
  var p3 : point = new point();
  p3.x = p1.x + p2.x;
  p3.y = p1.y + p2.y;
  return p3;
}

var a : point = new point();
a.x = 1;
a.y = 1;

var b : point = new point();
b.x = 1;
b.y = 1;

var c : point = new point();
c.x = 1;
c.y = 1;

writeln("a: (x = ", a.x, ", y = ", a.y, ")");
writeln("b: (x = ", b.x, ", y = ", b.y, ")");
writeln("c: (x = ", c.x, ", y = ", c.y, ")");

c = a + b;

writeln("c: (x = ", c.x, ", y = ", c.y, ")");
