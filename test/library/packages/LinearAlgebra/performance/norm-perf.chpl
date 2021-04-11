use LinearAlgebra, Random;
use Time;
use Math;

config const m = 3,
             iters=10,
             correctness=false;

config const testMatrixNorm=true;

const nbytes = numBytes(real);

var A: [1..m, 1..m] real;
var b: [1..m] real;

if(correctness) {

    A[1,..] = [1.0, 0.0, 0.0];
    A[2,..] = [1.0, 2.0, 0.0];
    A[3,..] = [1.0, 3.0, 5.0];

    const frobNorm = norm(A, normType.normFrob);
    const l1Norm = norm(A, normType.norm1);
    const linfNorm = norm(A, normType.normInf);
    
    writeln(frobNorm);
    writeln(l1Norm);
    writeln(linfNorm);
}
else{

    var t: Timer;

    writeln('============================');
    writeln('Norm Performance Test');
    writeln('============================');
    writeln('iters                  : ', iters);
    writeln('square matrix size     : ', m);
    writeln('MB                     : ', (nbytes*m*m) / 10**6);
    writeln('numLocales     : ', numLocales);
    writeln();

    if(testMatrixNorm){

        fillRandom(A, seed=10);

        for 1..iters {
            t.start();
            const frobNorm = norm(A, normType.normFrob);
            t.stop();
        }
        
        writeln('LinearAlgebra.norm_frob:', t.elapsed()/iters);
        t.clear();

        for 1..iters {
            t.start();
            const l1Norm = norm(A, normType.norm1);
            t.stop();
        }

        writeln('LinearAlgebra.norm_l1:', t.elapsed()/iters);
        t.clear();

        for 1..iters {
            t.start();
            const infNorm = norm(A, normType.normInf);
            t.stop();
        }
        
        writeln('LinearAlgebra.norm_inf:', t.elapsed()/iters);
        t.clear();
    }
    else{
        
        fillRandom(b, seed=10);

        for 1..iters {
            t.start();
            const frobNorm = norm(b, normType.normFrob);
            t.stop();
        }
        
        writeln('LinearAlgebra.norm_frob:', t.elapsed()/iters);
        t.clear();

        for 1..iters {
            t.start();
            const l1Norm = norm(b, normType.norm1);
            t.stop();
        }

        writeln('LinearAlgebra.norm_l1:', t.elapsed()/iters);
        t.clear();

        for 1..iters {
            t.start();
            const infNorm = norm(b, normType.normInf);
            t.stop();
        }
        
        writeln('LinearAlgebra.norm_inf:', t.elapsed()/iters);
        t.clear();
    }
}
