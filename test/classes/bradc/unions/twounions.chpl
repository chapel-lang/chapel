union myfirstunion {
  var x: int;
  var y: real;
}

union mysecondunion {
  var x: int;
  var y: real;
}

var a: myfirstunion;
var b: mysecondunion;

a.x = 1;
b.y = 1.2;

writeln("a is: ", a, ", b is: ", b);
