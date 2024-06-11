use GPU;

on here.gpus[0] {
  fooFn(128);
  fooAttr(128);
}

proc fooFn(blockSize: int(32)) {
  var A: [1..10] int;
  @assertOnGpu
  foreach a in A {
    setBlockSize(blockSize);
    a = 1;
  }

  writeln(A);
}

proc fooAttr(blockSize: int(32)) {
  var A: [1..10] int;
  @assertOnGpu
  @gpu.blockSize(blockSize)
  foreach a in A {
    a = 1;
  }

  writeln(A);
}
