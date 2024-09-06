
on here.gpus(0) {
  write  ("index               :");
  for i in 1..32 do writef("%4i", i);
  writeln();

  var A: [1..32] int = 100;
  writeln("initially      (100): ", A);

  foreach i in 5..28 do
    A[i] = 200 + __primitive("gpu threadIdx x");
  writeln("blockSize dflt (200): ", A);

  @gpu.blockSize(8)
  foreach i in 1..24 do
    A[i] = 300 + __primitive("gpu threadIdx x"):int;
  writeln("blockSize=8    (300): ", A);

  @gpu.itersPerThread(4)
  foreach i in 5..28 do
    A[i] = 400 + __primitive("gpu threadIdx x"):int;
  writeln("itersPerThr=4  (400): ", A);

  @gpu.itersPerThread(3)
  @gpu.blockSize(5)
  foreach i in 1..25 do
    A[i] = 500 + __primitive("gpu threadIdx x"):int;
  writeln("blsz=5 ipt=3   (500): ", A);

  @gpu.blockSize(2)
  @gpu.itersPerThread(3)
  foreach (i,j,k) in zip(2..8, 11..17, 19..25) {
    A[i] = 610 + __primitive("gpu threadIdx x"):int;
    A[j] = 620 + __primitive("gpu blockIdx x"):int;
    A[k] = 630 + __primitive("gpu gridDim x"):int;
  }
  writeln("blsz=2 ipt=3   (600): ", A);
}
