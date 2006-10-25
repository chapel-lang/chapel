record point {
  var x : real;
  var y : real;
}

var p : point = point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");
