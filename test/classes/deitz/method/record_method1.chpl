record point {
  var x : real;
  var y : real;
  proc hello() {
    writeln("Hi.  I'm a point.");
  }
}

var p : point = new point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

p.hello();
