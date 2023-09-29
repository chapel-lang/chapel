// this test is similar to blockSize-int32: setBlockSize was failing because of
// a type coercion issue. This is trying type coercion in a more general sense.
// It looks like the original bug was about the primitive that setBlockSize
// uses, and we can do type coercion in general. This test locks in the basic
// behavior nonetheless
use GPU;

on here.gpus[0] {
  foo(128);
}

proc bar(x: int) {
  return x+2;
}

proc foo(arg: int(32)) {
  var A: [1..10] int;
  @assertOnGpu
  foreach a in A {
    a = bar(arg);
  }

  writeln(A);
}
