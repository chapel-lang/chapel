
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

// This following checks correctness of thread/block/grid indices/dimensions.
// These should be calculated as follows, using ceiling divisions:
//   num.Threads = numIndices / itersPerThread
//   gridSize    = num.Blocks = num.Threads / blockSize
//   threadIdx   = 0..<blockSize, each index repeated itersPerThread times
//   blockIdx    = 0..<gridSize, each index (blockSize * itersPerThread) times

proc show(name: string, arr) {
  writef("%11s:", name);
  for elm in arr do writef("%3i", elm);
  writef("\n");
}

proc test(numIndices: int, itersPerThread: int, blockSize: int, param cyclic) {
  on here.gpus(0) {
    var threadIdx, blockDim, blockId, gridDim: [1..numIndices] int;

    @gpu.blockSize(blockSize)
    @gpu.itersPerThread(itersPerThread, cyclic)
    foreach i in 1..numIndices {
      threadIdx[i] = __primitive("gpu threadIdx x");
      blockDim[i] = __primitive("gpu blockDim x");
      blockId[i] = __primitive("gpu blockIdx x");
      gridDim[i] = __primitive("gpu gridDim x");
    }

    // all entries in 'blockDim' and 'gridDim' should be the same
    for elm in blockDim do
      if elm != blockDim[1] then
        { show("blockDim", blockDim); break; }
    for elm in gridDim do
      if elm != gridDim[1] then
        { show("gridDim", gridDim); break; }

    writef("numIndices %i  itersPerThread %i  blockSize %i/%i  gridSize %i\n",
           numIndices, itersPerThread, blockSize, blockDim[1], gridDim[1]);
    show("threadIdx", threadIdx);
    show("blockId",   blockId);
    writeln();
  }
}


writeln();

for param cyclic in false..true {
  test(32, 1, 3, cyclic);
  test(32, 2, 3, cyclic);
  test(32, 3, 3, cyclic);
  test(32, 4, 3, cyclic);
  test(32, 5, 3, cyclic);

  test(32, 8, 1, cyclic);
  test(32, 8, 2, cyclic);
  test(32, 8, 3, cyclic);
  test(32, 8, 4, cyclic);
  test(32, 8, 5, cyclic);
}
