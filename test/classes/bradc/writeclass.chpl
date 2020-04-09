class myclass {
  var x: int;
  var y: real;
}

var a: unmanaged myclass = new unmanaged myclass();
var b: unmanaged myclass = new unmanaged myclass();

writeln("a is: ", a, ", b is: ", b);
writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");
writeln();

a.x = 1;
a.y = 2.3;

b.x = 4;
b.y = 5.6;

writeln("a is: ", a, ", b is: ", b);
writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");
writeln();

delete a;

a   = b;
a.x = 7;
b.y = 8.9;

writeln("a is: ", a, ", b is: ", b);
writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");

delete a;
