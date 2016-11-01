// aif3.chpl
// test if expression of arrays where arrays
// are arguments

config const useA = true;

proc update(ref x)
{
  x[3] = 3;
}

proc test(A, B) {
  update( if useA then A else B );

  writeln(A);
  writeln(B);
}

proc run() {
  var A:[1..3] int;
  A[1] = 1;
  var B:[1..3] int;
  B[1] = 2;

  test(A, B);
}

run();
