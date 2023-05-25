use Time;
use ResultDB;
use IO.FormattedIO;
use GpuDiagnostics;
use MemDiagnostics;
use GPU;

config const passes = 1; //10;
config const alpha = 1.75: real(32);
config const noisy = false;
config var tolerance = 1;
config const output = true;
config const perftest = false;

proc main() {
    var timer: stopwatch;
    var kernelTimer: stopwatch;

    // Numbers just copied from cuda version
    // 256K through 8M bytes. (scaled down by 1024)
    const blockSizes = if(perftest) then [64, 16384] else [64, /*128, 256, 512, 1024, 2048, 4096, 8192,*/ 16384];
    param maxProblemSize = 16384;
    param numMaxFloats = 1024 * maxProblemSize / numBytes(real(32));
    param halfNumFloats = numMaxFloats/2;
    const maxBlockSize = blockSizes.last * 1024 / numBytes(real(32));

    var hos: [0..#numMaxFloats] real(32);
    //startVerboseMem();
    if !perftest then startGpuDiagnostics();

    var flopsDB = new ResultDatabase("TriadFlops", "GFLOP/s");
    var bdwthDB = new ResultDatabase("TriadBdwth", "GB/s");
    var triadDB = new ResultDatabase("Triad Time", "sec");
    var kernelDB = new ResultDatabase("Kernel Time", "sec");

    var bwBlockSize64 = 0.0;
    var bwBlockSize16384 = 0.0;

    on here.gpus[0] {
        var kernelLaunches = 0;

        // Make A's
        var A0 : [0..#maxBlockSize] real(32);
        var A1 : [0..#maxBlockSize] real(32);

        // ... Make B's and C's ....
        var B0 : [0..#maxBlockSize] real(32);
        var B1 : [0..#maxBlockSize] real(32);

        var C0 : [0..#maxBlockSize] real(32);
        var C1 : [0..#maxBlockSize] real(32);

        // N passes determined by a config const
        for pass in 0..#passes{
            for blkSize in blockSizes {
                // Populate host with arbitrary memory
                // The reference (CUDA) implementation of SHOC fills hos
                // with random values, in our implementation we fill with
                // known sample values so we can later verify the result.
                // This difference shouldn't impact performance.
                for i in 0..#halfNumFloats {
                    hos[i] = i:int(32)%16:int(32) + 0.12:real(32);
                    hos[halfNumFloats+i] = hos[i] ;
                }

                // Number of floats that fit in the number of bytes denoted
                // by problem size. Converting MB to Bytes and dividing by
                // size of real(32)
                var elemsInBlock = blkSize * 1024 / numBytes(real(32));

                if noisy then
                writeln(" >>> Executing Triad with arrays of length ", numMaxFloats,
                        " and block size of ", elemsInBlock, " elements");

                var A0, A1, B0, B1, C0, C1 : [0..<elemsInBlock] real(32);

                timer.start();

                var currentIdx = 0;
                while(currentIdx + elemsInBlock <= numMaxFloats) {
                  const blk0Range = currentIdx..#elemsInBlock;
                  const blk1Range = (currentIdx+elemsInBlock)..#elemsInBlock;

                  // Populate left hand synchronously
                  A0 = hos[blk0Range];
                  B0 = hos[blk0Range];

                  // Populate right half of array in background
                  var ev1, ev2 : GpuAsyncCommHandle;
                  if(currentIdx + elemsInBlock*2 <= numMaxFloats) {
                    ev1 = asyncGpuComm(A1, hos[blk1Range]);
                    ev2 = asyncGpuComm(B1, hos[blk1Range]);
                  }

                  // Triad computation for left half
                  foreach i in 0..<elemsInBlock {
                    assertOnGpu();
                    C0[i] = A0[i] + alpha * B0[i];
                  }

                  // Copy result back to host asynchronously
                  var ev3 = asyncGpuComm(hos[blk0Range], C0);

                  // Triad computation for right half
                  if(currentIdx + elemsInBlock*2 <= numMaxFloats) {
                    gpuCommWait(ev1);
                    gpuCommWait(ev2);
                    foreach i in 0..<elemsInBlock {
                      assertOnGpu();
                      C1[i] = A1[i] + alpha * B1[i];
                    }
                  }

                  hos[blk1Range] = C1;
                  gpuCommWait(ev3);

                  currentIdx += (elemsInBlock*2);
                }
                timer.stop();

                var timeElapsedInNanoseconds = timer.elapsed() * 1e9;
                var triad = (numMaxFloats * 2.0) / timeElapsedInNanoseconds;
                var bdwth = (numMaxFloats * numBytes(real(32)) * 3.0)
                /timeElapsedInNanoseconds;

                if noisy {
                    writeln("TriadFlops\t",
                    "Block",blkSize, "KB\t",
                    "GFLOP/s\t",
                    triad);

                    writeln("TriadBdwth\t",
                    "Block",blkSize, "KB\t",
                    "GB/s\t",
                    bdwth);

                    writeln("Triad Time : ", timer.elapsed());
                    writeln("Kernel Time : ", kernelTimer.elapsed());
                    writeln("Kernel Launches : ", kernelLaunches);
                }

// The following will introduce the following error on program termination when built with
// GPU_MEM_STRATEGY array_on_device
//                flopsDB.addToDatabase("%{#####}".format(blkSize), triad);
//                bdwthDB.addToDatabase("%{#####}".format(blkSize), bdwth);
//                triadDB.addToDatabase("%{#####}".format(blkSize), timer.elapsed());
//                kernelDB.addToDatabase("%{#####}".format(blkSize), kernelTimer.elapsed());

                if(blkSize == 64) {
                  bwBlockSize64 = bdwth;
                } else {
//                  assert(blkSize == 16384);
                  bwBlockSize16384 = bdwth;
                }

                kernelLaunches = 0;
                kernelTimer.clear();
                timer.clear();

                // Error Checking for the correct anwer
                // Since we gave the kernel arbitrary numbers we know what the answer
                // to the computation is
                // The formula used to calculate the index i on hos was
                // hos[i] = i%16 + 0.12;
                // Therefore the numbers in host repeat the following sequence over
                // and over: 0.12 1.12 2.12 3.12 4.12 5.12 6.12 7.12 8.12 9.12 10.12 11.12 12.12. 13.12. 14.12 15.12
                // The computation was
                // hos[i] = hos[i] * alpha + hos[i]
                // Therefore the answers should be equal to that
                // Each hos[i] after the computation is therefore as follows
                // hos[i] = (i%16+0.12) * alpha + (i%16+0.12)
                // The first expected answer, for example, will then be 0.12*1.75 + 0.12 = 0.33
                // Remember this was only for half of the memory.
                // The other half was just a copy of the first half
                // Check that on the CPU instead of the GPU
                for i in 0..#halfNumFloats {
                    var expected:real(32) = (i:int(32)%16:int(32) + 0.12:real(32)) * alpha + (i:int(32)%16:int(32) + 0.12:real(32));
                    var actual = hos[i];
                    if(noisy && !isclose(actual, expected)) {
                        writeln("Pass: ", pass,
                        "\nBlock Size:", blkSize,
                        "\nIndex: ", i,
                        "\nExpected: ", expected, "\nActual: ", actual);
                    }
                    assert(isclose(actual , expected));
                      assert(hos[halfNumFloats+i] == hos[i]);
                    }
            }
        }
    }

    //stopVerboseMem();
    if(output) {
        flopsDB.printDatabaseStats();
        bdwthDB.printDatabaseStats();
        triadDB.printDatabaseStats();
        kernelDB.printDatabaseStats();
    }
    if(perftest){
//        bdwthDB.printPerfStats();
//        triadDB.printPerfStats();
//        kernelDB.printPerfStats();
      writeln("BW block size 64 = ", bwBlockSize64, " GB/s");
      writeln("BW block size 16384 = ", bwBlockSize16384, " GB/s");
    }
    else {
      stopGpuDiagnostics();
      writeln(getGpuDiagnostics());
    }
}
