union myfirstunion {
  var x: integer;
  var y: float;
}

union mysecondunion {
  var x: integer;
  var y: float;
}

var a: myfirstunion;
var b: mysecondunion;

a.x = 1;
b.y = 1.2;

writeln("a is: ", a, ", b is: ", b);
