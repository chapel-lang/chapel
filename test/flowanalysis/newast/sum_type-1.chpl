class point {
  var x : real;
  var y : real;
}

class cpoint : point {
  var color : int;
}

proc length(a) {
  return a.x * a.y;
}

var p : point = new point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

var cp : cpoint = new cpoint();

cp.color = 5;
cp.x = 2.2;
cp.y = 3.3;

writeln("cp { ", cp.x, ", ", cp.y, ", ", cp.color, " }");

writeln("length p = ", length(p));
writeln("length cp = ", length(cp));
