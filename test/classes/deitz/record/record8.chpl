record point {
  var x : real;
  var y : real;
}

var p : point;
var p2 : point;

p.x = 3.0;
p.y = 4.5;

p2.x = 5.0;

writeln("p { ", p.x, ", ", p.y, " }");
writeln("p2 { ", p2.x, ", ", p2.y, " }");
