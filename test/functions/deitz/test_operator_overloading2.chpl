record point {
  var x : integer;
  var y : integer;
}

function ==(p1 : point, p2 : point) : boolean {
  if p1.x != p2.x then return false;
  if p1.y != p2.y then return false;
  return true;
}

var a : point = point();
a.x = 1;
a.y = 1;

var b : point = point();
b.x = 1;
b.y = 1;

var c : point = point();
c.x = 2;
c.y = 2;

writeln("a: (x = ", a.x, ", y = ", a.y, ")");
writeln("b: (x = ", b.x, ", y = ", b.y, ")");
writeln("c: (x = ", c.x, ", y = ", c.y, ")");

writeln(a == b);
writeln(a == c);
writeln(b == c);
