// aif-slice-param.chpl
// test if expression of arrays where the
// conditional is param and where the if expression
// is returning a slice

config param param_useA = true;

proc update(ref x)
{
  x[3] = 3;
}

var A:[1..3] int;
A[1] = 1;
var B:[1..3] int;
B[1] = 2;


proc run() {
  update( if param_useA then A[2..3] else B[3..3] );

  writeln(A);
  writeln(B);
}

run();
