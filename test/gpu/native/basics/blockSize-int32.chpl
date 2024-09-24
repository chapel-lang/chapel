use GPU;

on here.gpus[0] {
  fooAttr(128);
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
