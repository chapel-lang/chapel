use Time;
use ResultDB;
use IO.FormattedIO;
use GpuDiagnostics;

config const passes = 10;
config const alpha = 1.75: real(32);
config const noisy = false;
config var tolerance = 1;
config const output = true;
config const perftest = false;


proc main(){
    var timer: stopwatch;
    var kernelTimer: stopwatch;

    // Numbers just copied from cuda version
    // 256K through 8M bytes. (scaled down by 1024)
    const blockSizes = if(perftest) then [64, 16384] else [64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384];
    param maxProblemSize = 16384;
    param numMaxFloats = 1024 * maxProblemSize / numBytes(real(32));
    param halfNumFloats = numMaxFloats/2;
    // Create an two arrays on the GPU and populate them with number
    var flopsDB = new ResultDatabase("TriadFlops", "GFLOP/s");
    var bdwthDB = new ResultDatabase("TriadBdwth", "GB/s");
    var triadDB = new ResultDatabase("Triad Time", "sec");
    var kernelDB = new ResultDatabase("Kernel Time", "sec");

    var hos: [0..#numMaxFloats] real(32);
    startGpuDiagnostics();
    on here.gpus[0] {
        var kernelLaunches = 0;
        for pass in 0..#passes{
            if noisy then writeln("################################################################\n Pass Number ", pass);
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
                // Calculate the real(32) that will fir in problem KB
                var elemsInBlock = blkSize * 1024 / numBytes(real(32));

                // Create the arrays in unified memory
                var A, B, C :[0..#elemsInBlock] real(32);

                // Pre slice all the slices of host that we will need so the needed to slice
                // is not counted.

                // Calc the number of iterations needed
                var numIterations = maxProblemSize/ blkSize;
                // Make array of sliceshost arrays
                var hostArraysArray: [0..#numIterations][0..#elemsInBlock] real(32);
                for i in 0..#numIterations {
                    hostArraysArray[i] = hos[elemsInBlock*i..#elemsInBlock];
                }

                if noisy then
                writeln(" >>> Executing Triad with arrays of length ", numMaxFloats,
                        " and block size of ", elemsInBlock, " elements");
                // Checking the hostArraysArray
                if noisy then
                writeln("For above block size we expect ", numIterations, " sliced arrays"
                    , " and we have ", hostArraysArray.domain);
                if noisy then
                writeln("Starting timer with previously elapsed : ", timer.elapsed());
                timer.start();
                // Do numIterations iterations to calcuate the whole hos array
                for i in 0..#numIterations {
                    A = hostArraysArray[i];
                    B = hostArraysArray[i];
                    kernelTimer.start();
                    C = A * alpha + B; // Kernel
                    kernelTimer.stop();
                    hostArraysArray[i] = C; // Copy Back
                }
                timer.stop();
                if noisy then
                writeln("Finished triad with elapsed time : ", timer.elapsed());
                kernelLaunches = numIterations;
                // Copy hostArraysArray into hos back for error checking
                for i in 0..#numIterations {
                    hos[elemsInBlock*i..#elemsInBlock] = hostArraysArray[i];
                }

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

                flopsDB.addToDatabase("%{#####}".format(blkSize), triad);
                bdwthDB.addToDatabase("%{#####}".format(blkSize), bdwth);
                triadDB.addToDatabase("%{#####}".format(blkSize), timer.elapsed());
                kernelDB.addToDatabase("%{#####}".format(blkSize), kernelTimer.elapsed());
                kernelLaunches = 0;
                kernelTimer.clear();
                timer.clear();
                if noisy then
                writeln("Elapsed time after clearing: ", timer.elapsed());
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
    stopGpuDiagnostics();
    writeln(getGpuDiagnostics());
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
}
