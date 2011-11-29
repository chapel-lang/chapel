//
// This test demonstrates what goes wrong when a user forgets
// our reduction precedence rules;  nicely it blows up big.
//
proc foo(x: int) {
  writeln("x is: ", x);
}

var A: [1..3] int = (1, 2, 3);
var B: [1..3] int = (4, 5, 6);

var total: int = + reduce A * B;
