use Time;
use ResultDB;
use IO.FormattedIO;
use GpuDiagnostics;


config const passes = 10;
config const alpha = 1.75: real(32);
config const noisy = false;
config const output = true;
config const perftest = false;

proc main(){
  var flopsDB = new ResultDatabase("TriadFlops", "GFLOP/s");
  var bdwthDB = new ResultDatabase("TriadBdwth", "GB/s");
  var triadDB = new ResultDatabase("Triad Time", "sec");
  var kernelDB = new ResultDatabase("Kernel Time", "sec");

  var timer: stopwatch;
  var kernelTimer: stopwatch;
  // Just do the whole computation once because overhead of GPU Launches is high
  param maxProblemSize = 16384;
  param numMaxFloats = (1024 * maxProblemSize / numBytes(real(32))) : int;
  param halfNumFloats = numMaxFloats/2;


  var A, B, C : [0..#numMaxFloats] real(32);

  // Populate A and B with arbitrary memory
  // The reference (CUDA) implementation of SHOC fills hos
  // with random values, in our implementation we fill with
  // known sample values so we can later verify the result.
  // This difference in values shouldn't impact performance.
  forall i in 0..#halfNumFloats with (ref A, ref B) {
    A[i] = i:int(32)%16:int(32) + 0.12:real(32);
    A[halfNumFloats+i] = A[i] ;
    B[i] = A[i];
    B[halfNumFloats+i] = A[halfNumFloats+i];
  }

  const host = here;

  if !perftest then startGpuDiagnostics();

  on here.gpus[0] {
    var ADev, BDev, CDev : [0..#numMaxFloats] real(32);

    for pass in 0..#passes{
      timer.start();
      ADev = A;
      BDev = B;

      kernelTimer.start();
      // forall i in 0..#numMaxFloats {
      //     C[i] = A[i] * alpha + B[i];
      // }

      // OR

      // forall (a,b,c) in zip(A,B,C){
      //     c = a*alpha+b;
      // }

      // OR

      CDev = ADev * alpha + BDev;
      kernelTimer.stop();

      C = CDev;
      timer.stop();

      var timeElapsedInNanoseconds = timer.elapsed() * 1e9;
      var triad = (numMaxFloats * 2.0) / timeElapsedInNanoseconds;
      var bdwth = (numMaxFloats * numBytes(real(32)) * 3.0) /
                  timeElapsedInNanoseconds;

      if noisy {
        writeln("TriadFlops\t",
            "Block",maxProblemSize, "KB\t",
            "GFLOP/s\t",
            triad);

        writeln("TriadBdwth\t",
            "Block",maxProblemSize, "KB\t",
            "GB/s\t",
            bdwth);

        writeln("Triad Time : ", timer.elapsed());
      }

      flopsDB.addToDatabase("%{#####}".format(maxProblemSize), triad);
      bdwthDB.addToDatabase("%{#####}".format(maxProblemSize), bdwth);
      triadDB.addToDatabase("%{#####}".format(maxProblemSize), timer.elapsed());
      kernelDB.addToDatabase("%{#####}".format(maxProblemSize),
                             kernelTimer.elapsed());

      on host {
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
          var actual = C[i];
          if(noisy && !isClose(actual, expected)) {
            writeln("Pass: ", pass,
                "\nBlock Size:", maxProblemSize,
                "\nIndex: ", i,
                "\nExpected: ", expected, "\nActual: ", actual);
          }
          assert(isClose(actual , expected));
          assert(C[halfNumFloats+i] == C[i]);
        }
      }
    }
  }
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
    assertGpuDiags(kernel_launch_um=4, kernel_launch_aod=4);
  }
}
