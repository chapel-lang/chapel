class myclass {
  var x: integer;
  var y: float;
}

var a: myclass;
var b: myclass;

a.x = 1;
a.y = 2.3;

b.x = 4;
b.y = 5.6;

writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");

a = b;

a.x = 7;
b.y = 8.9;

writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");
