record intcoord {
  var x: int;
  var y: int;
}

record realcoord {
  var w: real;
  var z: real;
}

var a:intcoord;
var b:realcoord;

a.x = 1;
a.y = 2;
b.w = 3.4;
b.z = 5.6;

writeln("a is: ", a);
writeln("b is: ", b);

