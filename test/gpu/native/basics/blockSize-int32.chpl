use GPU;

on here.gpus[0] {
  foo(128);
}

proc foo(blockSize: int(32)) {
  var A: [1..10] int;
  @assertOnGpu
  foreach a in A {
    setBlockSize(blockSize);
    a = 1;
  }

  writeln(A);
}
