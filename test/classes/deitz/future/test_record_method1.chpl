record point {
  var x : float;
  var y : float;
  function hello() {
    writeln("Hi.  I'm a point.");
  }
}

var p : point = point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

p.hello();
