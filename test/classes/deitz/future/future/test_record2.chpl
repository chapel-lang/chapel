record point {
  var x : float;
  var y : float;
}

record cpoint {
  var color : integer;
  var coordinate : point;
}

var p : point = point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

var cp : cpoint = cpoint();

cp.color = 5;
cp.coordinate.x = 2.2;
cp.coordinate.y = 3.3;

writeln("cp { ", cp.coordinate.x, ", ", cp.coordinate.y, ", ", cp.color, " }");
