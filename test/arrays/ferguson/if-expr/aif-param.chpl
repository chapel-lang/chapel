// aif-param.chpl
// test if expression of arrays where the
// conditional is param
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
  update( if param_useA then A else B );

  writeln(A);
  writeln(B);
}

run();
