record point {
  var x : real;
  var y : real;
}

record cpoint {
  var color : int;
  var coordinate : point;
}

var p : point = new point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

var cp : cpoint = new cpoint();

cp.color = 5;
cp.coordinate.x = 2.2;
cp.coordinate.y = 3.3;

writeln("cp { ", cp.coordinate.x, ", ", cp.coordinate.y, ", ", cp.color, " }");

var cp2 : cpoint = new cpoint();

cp2.color = 5;
cp2.coordinate.x = 2.5;
cp2.coordinate.y = 3.3;

writeln(cp == cp2);

cp2.coordinate.x = 2.2;

writeln(cp == cp2);
