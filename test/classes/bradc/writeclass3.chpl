class myclass {
  var x: integer = 42;
  var y: float = 4.2;
}

var a: myclass = myclass();
var b: myclass = myclass();

write("a is: ", a, "b is: ", b);

a = nil;
b = nil;

write("a is: ", a, "b is: ", b);
