class myclass {
  var x: int = 42;
  var y: real = 4.2;
}

var a: borrowed myclass?;
var b: unmanaged myclass?;

writeln("a is: ", a);
writeln("b is: ", b);
