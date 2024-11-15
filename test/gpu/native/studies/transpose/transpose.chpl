use CTypes;
use GpuDiagnostics;
use GPU;
use Time;
use Math;

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

inline proc transposeNaive(original, ref output) {
  @assertOnGpu
  foreach (x,y) in original.domain {
    output[y,x] = original[x,y];
  }
}

inline proc transposeClever(original, ref output) {
  @assertOnGpu
  @gpu.blockSize(blockSize * blockSize)
  foreach 0..<original.size {
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

pragma "GPU kernel"
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

inline proc transposeLowLevel(original, ref output) {
  var cfg = __primitive("gpu init kernel cfg 3d",
                        /*fn*/ "transposeMatrix":chpl_c_string,
                        /*grd_dims*/ sizeX / blockSize, sizeY / blockSize, 1,
                        /*blk_dims*/ blockSize, blockSize, 1,
                        /*args*/4,
                        /*pids*/0,
                        /*reductions*/0,
                        /*host_reg_vars*/0);

  // 1 is an enum value that says: "pass the address of this to the
  //   kernel_params, while not offloading anything".
  __primitive("gpu arg", cfg, c_ptrTo(output), 1);
  __primitive("gpu arg", cfg, c_ptrToConst(original), 1);
  __primitive("gpu arg", cfg, sizeX, 1);
  __primitive("gpu arg", cfg, sizeY, 1);

  __primitive("gpu kernel launch",
              /* kernel config */ cfg);
}

var originalHost: [0..#sizeX, 0..#sizeY] dataType;
var outputHost: [0..#sizeY, 0..#sizeX] dataType;
forall (a, (x,y)) in zip(originalHost, originalHost.domain) {
  a = x*sizeY + y;
}

var timer: stopwatch;


on here.gpus[0] {
  var originalDev: [0..#sizeX, 0..#sizeY] dataType;
  var outputDev: [0..#sizeY, 0..#sizeX] dataType;

  originalDev = originalHost;

  // Make sure a is on device if we're using unified memory.
  foreach a in originalDev do a = a + 1;

  for 1..#numTrials {
    timer.start();
    select impl {
      when naive do transposeNaive(originalDev, outputDev);
      when clever do transposeClever(originalDev, outputDev);
      when lowlevel do transposeLowLevel(originalDev, outputDev);
    }
    timer.stop();
  }

  outputHost = outputDev;
}

var elapsed = timer.elapsed() / numTrials;

if perftest {
var sizeInBytes = originalHost.size * numBytes(dataType);
  writeln("Wall clock time (s): ", elapsed);
  var sizeInGb = sizeInBytes / (1000.0 * 1000.0 * 1000.0);
  var gibPerSec = sizeInGb / elapsed;
  // GiB/s is the precise metric here. However, GB/s can be used interchangably
  // and that's how we started testing. Changing it confuses the test system.
  // Note that we report this as GiB/s in the relevant plot.
  writeln("Performance (GB/s): ", gibPerSec);
}

var passed = true;
for (x,y) in originalHost.domain {
  // -1 because we incremented the input once as a warmup
  if !isClose(originalHost[x,y], outputHost[y,x]-1) {
    writeln("Incorrect output at ", (x,y),
            ". Expected ", originalHost[x,y],
            ", got ", outputHost[y,x]);
    passed = false;
  }
}
writeln(if passed then "Passed" else "Failed");
