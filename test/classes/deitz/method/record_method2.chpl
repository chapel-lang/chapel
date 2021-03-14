record point {
  var x : real;
  var y : real;
  proc setx(newx : real) {
    x = newx;
  }
}

var p : point = new point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

p.setx(6.5);

writeln("p { ", p.x, ", ", p.y, " }");
