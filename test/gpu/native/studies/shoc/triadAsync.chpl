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
  var flopsDB = new ResultDatabase("TriadFlops", "GFLOP/s");
  var bdwthDB = new ResultDatabase("TriadBdwth", "GB/s");
  var triadDB = new ResultDatabase("Triad Time", "sec");
  var kernelDB = new ResultDatabase("Kernel Time", "sec");

  //startVerboseMem();
  if !perftest then startGpuDiagnostics();

  var kernelLaunches = 0;

  // N passes determined by a config const
  for pass in 0..#passes {
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

      var triad: real;
      var bdwth: real;

      on here.gpus[0] {
        // Make A's
        var A0 : [0..#elemsInBlock] real(32);
        var A1 : [0..#elemsInBlock] real(32);

        // ... Make B's and C's ....
        var B0 : [0..#elemsInBlock] real(32);
        var B1 : [0..#elemsInBlock] real(32);

        var C0 : [0..#elemsInBlock] real(32);
        var C1 : [0..#elemsInBlock] real(32);


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

          kernelTimer.start();
          // Triad computation for left half
          @assertOnGpu()
          foreach i in 0..<elemsInBlock {
            C0[i] = A0[i] + alpha * B0[i];
          }
          kernelTimer.stop();

          // Copy result back to host asynchronously
          var ev3 = asyncGpuComm(hos[blk0Range], C0);

          // Triad computation for right half
          if(currentIdx + elemsInBlock*2 <= numMaxFloats) {
            gpuCommWait(ev1);
            gpuCommWait(ev2);
            kernelTimer.start();
            @assertOnGpu()
            foreach i in 0..<elemsInBlock {
              C1[i] = A1[i] + alpha * B1[i];
            }
            kernelTimer.stop();
          }

          hos[blk1Range] = C1;
          gpuCommWait(ev3);

          currentIdx += (elemsInBlock*2);
        }
        timer.stop();

        const timeElapsedInNanoseconds = timer.elapsed() * 1e9;
        triad = (numMaxFloats * 2.0) / timeElapsedInNanoseconds;
        bdwth = (numMaxFloats * numBytes(real(32)) * 3.0) /timeElapsedInNanoseconds;

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
      } // end gpu on


      flopsDB.addToDatabase("%{#####}".format(blkSize), triad);
      bdwthDB.addToDatabase("%{#####}".format(blkSize), bdwth);
      triadDB.addToDatabase("%{#####}".format(blkSize), timer.elapsed());
      kernelDB.addToDatabase("%{#####}".format(blkSize), kernelTimer.elapsed());

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
        if(noisy && !isClose(actual, expected)) {
          writeln("Pass: ", pass,
              "\nBlock Size:", blkSize,
              "\nIndex: ", i,
              "\nExpected: ", expected, "\nActual: ", actual);
        }
        assert(isClose(actual , expected));
        assert(hos[halfNumFloats+i] == hos[i]);
      }
    } // for in blockSizes
  } // for in passes

  //stopVerboseMem();
  if(output) {
    flopsDB.printDatabaseStats();
    bdwthDB.printDatabaseStats();
    triadDB.printDatabaseStats();
    kernelDB.printDatabaseStats();
  }
  if(perftest){
    bdwthDB.printPerfStats();
    triadDB.printPerfStats();
    kernelDB.printPerfStats();
  }
  else {
    stopGpuDiagnostics();
    assertGpuDiags(kernel_launch_um=263, kernel_launch_aod=269,
                   host_to_device=258, device_to_host=128, device_to_device=0);
  }
}
