union myunion {
  var x: int;
  var y: real;
}

var a: myunion;
var b: myunion;

writeln("a is: ", a, ", b is: ", b);

a.x = 1;
b.y = 5.6;

writeln("a is: ", a, ", b is: ", b);

a = b;

writeln("a is: ", a, ", b is: ", b);

a.x = 7;
b.y = 8.9;

writeln("a is: ", a, ", b is: ", b);

