class point {
  var x : real;
  var y : real;

  proc speak() {
    writeln("Hello");
  }
}

class cpoint : point {
  var color : int;
}

var p : borrowed point = new borrowed point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

var cp : borrowed cpoint = new borrowed cpoint();

cp.color = 5;
cp.x = 2.2;
cp.y = 3.3;

writeln("cp { ", cp.x, ", ", cp.y, ", ", cp.color, " }");

cp.speak();
