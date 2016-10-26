// aif4.chpl
// test if expression of arrays using function calls
// returning arrays

config const useA = true;

proc update(ref x)
{
  writeln(x);
  x[3] = 3;
  writeln(x);
}


var A:[1..3] int;
proc makeA() {
  A[1] = 1;
  return A;
}

var B:[1..3] int;
proc makeB() {
  B[1] = 2;
  return B;
}

proc run() {
  update( if useA then makeA() else makeB() );

  writeln(A);
  writeln(B);
}

run();
