use CTypes;
use GpuDiagnostics;
use GPU;
use Time;

// This test implements matrix transpose operations based on the following example:
//
// https://www.nvidia.com/content/cudazone/cuda_sdk/Linear_Algebra.html#transpose
//
// In particular, the naive implType represents a naive matrix multiplication
// (one that just writes (i,j) to (j,i)). The clever and lowlevel versions
// represent the implementation that does memory coalescing. The lowlevel
// verion uses a GPU kernel launch primitive to get a 2D kernel (which matches
// the CUDA version more closely), whereas the clever version uses a 1D
// kernel that is implicitly launched from a foreach loop.
enum implType {
    naive, clever, lowlevel
}
use implType;

config const perftest = false;
config const numTrials = 10;
config const impl = naive;
config const sizeX = 2048*8,
             sizeY = 2048*8;
config param blockSize = 16;
config param blockPadding = 1;
config type dataType = real(32);

inline proc transposeNaive(original, output) {
  foreach (x,y) in original.domain {
    assertOnGpu();
    output[y,x] = original[x,y];
  }
}

inline proc transposeClever(original, output) {
  foreach 0..<original.size {
    assertOnGpu();
    setBlockSize(blockSize * blockSize);
    param paddedBlockSize = blockSize + blockPadding;
    var smArrPtr = createSharedArray(dataType, paddedBlockSize*blockSize);

    // The blockIdx and threadIdx can be computed from the index variable
    // of the loop if need be. However, using the GPU's underlying primitives
    // to get these values is cleaner (and less brittle in case the block
    // size changes).
    const blockIdx = __primitive("gpu blockIdx x"),
          threadIdx = __primitive("gpu threadIdx x");

    const blockIdxX = blockIdx % (sizeX / blockSize),
          blockIdxY = blockIdx / (sizeX / blockSize),
          // Swapping thread X/Y here is safe because these coordinates are
          // entirely "synthetic": all we need is a one-to-one correspondence
          // of 1D blocks and threads to 2D blocks and threads.
          //
          // However, swapping X/Y seems to have a significant performance
          // impact, for reasons yet unknown.
          threadIdxY = threadIdx % blockSize,
          threadIdxX = threadIdx / blockSize;
    var idxX = blockIdxX * blockSize + threadIdxX,
        idxY = blockIdxY * blockSize + threadIdxY;
    // Store the input data in transposed order into temporary array
    // i.e., the below is effectively smArrPtr[y][x] instead of
    // smArrPtr[x][y] for copy
    smArrPtr[paddedBlockSize * threadIdxX + threadIdxY] = original[idxX, idxY];

    // synchronize the threads
    syncThreads();

    // Swap coordinates and write back out
    idxX = blockIdxY * blockSize + threadIdxX;
    idxY = blockIdxX * blockSize + threadIdxY;
    output[idxX, idxY] = smArrPtr[paddedBlockSize * threadIdxY + threadIdxX];
  }
}

pragma "codegen for GPU"
pragma "always resolve function"
export proc transposeMatrix(odata: c_ptr(dataType), idata: c_ptr(dataType), width: int, height: int) {
  // Allocate extra columns for the shared 2D array to avoid bank conflicts.
  param paddedBlockSize = blockSize + blockPadding;
  var smArrPtr = createSharedArray(dataType, paddedBlockSize*blockSize);

  const blockIdxX = __primitive("gpu blockIdx x"),
        threadIdxX = __primitive("gpu threadIdx x"),
        blockIdxY = __primitive("gpu blockIdx y"),
        threadIdxY = __primitive("gpu threadIdx y");
  var idxX = blockIdxX * blockSize + threadIdxX,
      idxY = blockIdxY * blockSize + threadIdxY;

  // Store the input data in transposed order into temporary array
  // i.e., the below is effectively smArrPtr[y][x] instead of
  // smArrPtr[x][y] for copy
  if (idxX < width && idxY < height) {
    smArrPtr[paddedBlockSize * threadIdxX + threadIdxY] = idata[idxY * width + idxX];
  }

  // synchronize the threads
  syncThreads();

  // Swap coordinates and write back out
  idxX = blockIdxY * blockSize + threadIdxX;
  idxY = blockIdxX * blockSize + threadIdxY;
  if (idxX < height && idxY < width) {
    odata[idxY * height + idxX] = smArrPtr[paddedBlockSize * threadIdxY + threadIdxX];
  }
}

inline proc transposeLowLevel(original, output) {
  __primitive("gpu kernel launch",
          c"transposeMatrix",
          /* grid size */  sizeX / blockSize, sizeY / blockSize, 1,
          /* block size */ blockSize, blockSize, 1,
          /* kernel args */ c_ptrTo(output), c_ptrTo(original), sizeX, sizeY);
}

on here.gpus[0] {
  var original: [0..#sizeX, 0..#sizeY] dataType;
  var output: [0..#sizeY, 0..#sizeY] dataType;

  for (a, (x,y)) in zip(original, original.domain) {
    a = x*sizeY + y;
  }

  // Make sure a is on device if we're using unified memory.
  foreach a in original do a = a + 1;

  var timer: stopwatch;
  for 1..#numTrials {
    timer.start();
    select impl {
      when naive do transposeNaive(original, output);
      when clever do transposeClever(original, output);
      when lowlevel do transposeLowLevel(original, output);
    }
    timer.stop();
  }
  var elapsed = timer.elapsed() / numTrials;

  var sizeInBytes = original.size * numBytes(dataType);
  var sizeInGb = sizeInBytes / (1000.0 * 1000.0 * 1000.0);
  var gbPerSec = sizeInGb / elapsed;
  if perftest {
    writeln("Wall clock time (s): ", elapsed);
    writeln("Performance (GB/s): ", gbPerSec);
  }

  var passed = true;
  for (x,y) in original.domain {
    if original[x,y] != output[y,x] {
      writeln("Incorrect output at ", (x,y),
              ". Expected ", original[x,y],
              ", got ", output[y,x]);
      passed = false;
    }
  }
  writeln(if passed then "Passed" else "Failed");
}
