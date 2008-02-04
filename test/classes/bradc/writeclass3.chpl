class myclass {
  var x: int = 42;
  var y: real = 4.2;
}

var a: myclass = new myclass();
var b: myclass = new myclass();

writeln("a is: ", a, ", b is: ", b);

a.x = 24;
b.y = 2.4;

writeln("a is: ", a, ", b is: ", b);

a = nil;
b = nil;

writeln("a is: ", a, ", b is: ", b);
