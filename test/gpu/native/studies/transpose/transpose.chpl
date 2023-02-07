use CTypes;
use GPUDiagnostics;
use GPU;
use Time;

config const numTrials = 10;
config const useNaive = false;
config const sizeX = 2048*8,
             sizeY = 2048*8;
config param blockSize = 16;
config param blockPadding = 1;
config type dataType = real(32);

pragma "codegen for GPU"
pragma "always resolve function"
export proc transposeMatrix(odata: c_ptr(dataType), idata: c_ptr(dataType), width: int, height: int) {
  // Allocate extra columns for the shared 2D array to avoid bank conflicts.
  param paddedBlockSize = blockSize + blockPadding;
  var smVoidPtr = __primitive("gpu allocShared", numBytes(dataType)*paddedBlockSize*blockSize);
  var smArrPtr = smVoidPtr : c_ptr(dataType);

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
  __primitive("gpu syncThreads");

  // Swap coordinates and write back out
  idxX = blockIdxY * blockSize + threadIdxX;
  idxY = blockIdxX * blockSize + threadIdxY;
  if (idxX < height && idxY < width) {
    odata[idxY * height + idxX] = smArrPtr[paddedBlockSize * threadIdxY + threadIdxX];
  }
}

proc transposeTiled(original, output) {
  __primitive("gpu kernel launch",
          c"transposeMatrix",
          /* grid size */  sizeX / blockSize, sizeY / blockSize, 1,
          /* block size */ blockSize, blockSize, 1,
          /* kernel args */ c_ptrTo(output), c_ptrTo(original), sizeX, sizeY);
}

proc transposeNaive(original, output) {
  foreach (x,y) in original.domain do output[y,x] = original[x,y];
}

on here.gpus[0] {
  var original: [0..#sizeX, 0..#sizeY] dataType;
  var output: [0..#sizeY, 0..#sizeY] dataType;

  for (a, (x,y)) in zip(original, original.domain) {
    a = x*sizeY + y;
  }

  // Make sure a is on device if we're using unified memory.
  foreach a in original do a = a + 1;

  var elapsed = 0.0;
  var timer: stopwatch;
  for 1..#numTrials {
    timer.start();
    if useNaive {
      transposeNaive(original, output);
    } else {
      transposeTiled(original, output);
    }
    timer.stop();
    elapsed += timer.elapsed();
    timer.reset();
  }
  elapsed /= numTrials;

  var sizeInBytes = original.size * numBytes(dataType);
  var sizeInGb = sizeInBytes / (1000.0 * 1000.0 * 1000.0);
  var gbPerSec = sizeInGb / elapsed;
  writeln("Wall clock time (s): ", elapsed);
  writeln("Performance (GB/s): ", gbPerSec);

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
