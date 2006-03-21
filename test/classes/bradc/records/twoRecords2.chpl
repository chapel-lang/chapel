record intcoord {
  var x: int;
  var y: int;
}

record floatcoord {
  var w: float;
  var z: float;
}

var a:intcoord;
var b:floatcoord;

a.x = 1;
a.y = 2;
b.w = 3.4;
b.z = 5.6;

writeln("a is: ", a);
writeln("b is: ", b);

