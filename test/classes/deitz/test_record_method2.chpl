record point {
  var x : float;
  var y : float;
  function setx(newx : float) {
    x = newx;
  }
}

var p : point = point();

p.x = 3.0;
p.y = 4.5;

writeln("p { ", p.x, ", ", p.y, " }");

p.setx(6.5);

writeln("p { ", p.x, ", ", p.y, " }");
