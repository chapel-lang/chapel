// aif-slice.chpl
// test if expression of arrays where the if expression
// is returning a slice



config const useA = true;

proc update(ref x)
{
  x[3] = 3;
}

var A:[1..3] int;
A[1] = 1;
var B:[1..3] int;
B[1] = 2;


proc run() {
  update( if useA then A[2..3] else B[3..3] );

  writeln(A);
  writeln(B);
}

run();
