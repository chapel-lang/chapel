class myclass {
  var x: integer = 42;
  var y: float = 4.2;
}

var a: myclass = myclass();
var b: myclass = myclass();

writeln("a is: ", a, ", b is: ", b);

a.x = 24;
b.y = 2.4;

writeln("a is: ", a, ", b is: ", b);

a = nil;
b = nil;

writeln("a is: ", a, ", b is: ", b);
