class myclass {
  var x: int = 42;
  var y: real = 4.2;
}

var ownA = new owned myclass();
var a: borrowed myclass? = ownA.borrow();
var ownB = new owned myclass();
var b: borrowed myclass? = ownB.borrow();

writeln("a is: ", a, ", b is: ", b);

a!.x = 24;
b!.y = 2.4;

writeln("a is: ", a, ", b is: ", b);

a = nil;
b = nil;

writeln("a is: ", a, ", b is: ", b);
