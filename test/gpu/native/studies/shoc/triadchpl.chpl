use Time;
use ResultDB;
use IO.FormattedIO;
use GPUDiagnostics;

config const passes = 10;
config const alpha = 1.75: real(32);
config const noisy = false;
config const output = true;

proc main(){
    startGPUDiagnostics();
    on here.gpus[0] {
        var flopsDB = new ResultDatabase("TriadFlops", "GFLOP/s");
        var bdwthDB = new ResultDatabase("TriadBdwth", "GB/s");
        var triadDB = new ResultDatabase("Triad Time", "sec");
        var kernelDB = new ResultDatabase("Kernel Time", "sec");
        var timer: Timer;
        var kernelTimer: Timer;
        // Just do the whole computation once because overhead of GPU Launches is high
        param maxProblemSize = 16384;
        param numMaxFloats = (1024 * maxProblemSize / numBytes(real(32))) : int; // 4 is the size of real(32)
        param halfNumFloats = numMaxFloats/2;

        var A, B, C : [0..#numMaxFloats] real(32);

        for pass in 0..#passes{
            // Populate A and B with arbitrary memory
            // TODO: Change arbitrary to random
            forall i in 0..#halfNumFloats {
                A[i] = i:int(32)%10:int(32) + 0.12:real(32);
                A[halfNumFloats+i] = A[i] ;
                B[i] = A[i];
                B[halfNumFloats+i] = A[halfNumFloats+i];
            }

            timer.start();
            // forall i in 0..#numMaxFloats {
            //     C[i] = A[i] * alpha + B[i];
            // }

            // OR

            // forall (a,b,c) in zip(A,B,C){
            //     c = a*alpha+b;
            // }

            // OR

            C = A * alpha + B;

            timer.stop();
            var timeElapsedInNanoseconds = timer.elapsed() * 1e9;
            var triad = (numMaxFloats * 2.0) / timeElapsedInNanoseconds;
            var bdwth = (numMaxFloats * numBytes(real(32)) * 3.0) // 4 is size of real(32)
            /timeElapsedInNanoseconds;

            if noisy {
                writeln("TriadFlops\t",
                // "Block:%{#####}KB\t".format(maxProblemSize),
                "Block",maxProblemSize, "KB\t",
                "GFLOP/s\t",
                triad);

                writeln("TriadBdwth\t",
                // "Block:%{#####}KB\t".format(maxProblemSize),
                "Block",maxProblemSize, "KB\t",
                "GB/s\t",
                bdwth);

                writeln("Triad Time : ", timer.elapsed());
            }

            flopsDB.addToDatabase("%{#####}".format(maxProblemSize), triad);
            bdwthDB.addToDatabase("%{#####}".format(maxProblemSize), bdwth);
            triadDB.addToDatabase("%{#####}".format(maxProblemSize), timer.elapsed());
            timer.clear();

            // Error Checking for the correct anwer
            // Since we gave the kernel arbitrary numbers we know what the answer
            // to the computation is
            // The formula used to calculate the index i on hos was
            // A[i] = i%10 + 0.12; // Same for B
            // Therefore the numbers in host repeat the following sequence over
            // and over: 0.12 1.12 2.12 3.12 4.12 5.12 6.12 7.12 8.12 9.12
            // The computation was
            // C[i] = A[i] * alpha + B[i]
            // Therefore the answers should be equal to that
            // Each C[i] after the computation is therefore as follows
            // C[i] = (i%10+0.12) * alpha + (i%10+0.12)
            // Check that on the CPU instead of the GPU
            for i in 0..#halfNumFloats {
                // var expected = (i%10+0.12) * alpha + (i%10+0.12);
                // var actual = C[i];
                // if(!isclose(actual, expected)) {
                //     writeln("Index: ", i, "\nExpected: ", expected, "\nActual: ", actual);
                //     writeln(isclose(actual, expected));
                //     halt();
                // }
                assert(isclose(C[i] , (i%10+0.12) * alpha + (i%10+0.12)));
                assert(C[halfNumFloats+i] == C[i]);
            }
        }
        if(output) {
            flopsDB.printDatabaseStats();
            bdwthDB.printDatabaseStats();
            triadDB.printDatabaseStats();
        }
    }
    stopGPUDiagnostics();
    writeln(getGPUDiagnostics());
}
