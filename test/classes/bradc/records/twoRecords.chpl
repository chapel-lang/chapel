record intcoord {
  var x: integer;
  var y: integer;
}

record floatcoord {
  var x: float;
  var y: float;
}

var a:intcoord;
var b:floatcoord;

a.x = 1;
a.y = 2;
b.x = 3.4;
b.y = 5.6;

writeln("a is: ", a);
writeln("b is: ", b);

