module test_halt { // SJD: inserted to avoid name conflict with module
var x : integer = 13;
var y : float   = 99.9;

if (x == y) {
   writeln("x is equal to y.");
} else {
  halt("x and y are not equal!\n", "x = ", x, ", y = ", y);
}
}
