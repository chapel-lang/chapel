record intcoord {
  var x: int;
  var y: int;
}

record realcoord {
  var x: real;
  var y: real;
}

var a:intcoord;
var b:realcoord;

a.x = 1;
a.y = 2;
b.x = 3.4;
b.y = 5.6;

writeln("Before assignment:\n");
writeln("a is: ", a);
writeln("b is: ", b);

b = a;

writeln("After assignment:\n");
writeln("a is: ", a);
writeln("b is: ", b);
