class myclass {
  var x: int = 42;
  var y: real = 4.2;
}

var a: borrowed myclass? = nil;
var b: unmanaged myclass? = nil;

writeln("a is: ", a);
writeln("b is: ", b);
