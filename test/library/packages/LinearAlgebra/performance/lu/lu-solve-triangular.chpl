use Time;
use Math;
use LinearAlgebra, Random;
use LinearAlgebra.Sparse;

const eps = 1e-7;

config const m=3,
             iters=10,
             correctness=true;


const nbytes = numBytes(real);

proc substituteAndCheckEqual(A, x, b){
    var b_hat = dot(A, x);
    var totalDiff = + reduce(abs(b-b_hat));
    return totalDiff < eps;
}

var A: [1..m, 1..m] real;
var b: [1..m] real;
var x: [1..m] real;

if(correctness) {
    
    A[1,..] = [1.0, 0.0, 0.0];
    A[2,..] = [1.0, 2.0, 0.0];
    A[3,..] = [1.0, 3.0, 5.0];

    b = [1.0, 2.0, 3.0];

    var x = solve_tril(A, b, unit_diag=false);
    if(!substituteAndCheckEqual(A, x, b)) then
        writeln("Found incorrect solution");
    
    var y = solve_triu(A.T, b);
    if(!substituteAndCheckEqual(A.T, y, b)) then
        writeln("Found incorrect solution");
}
else{
    var t: Timer;

    writeln('=========================================');
    writeln('  Solve Triangular Performance Test');
    writeln('=========================================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (nbytes*m*m) / 10**6);
    writeln();

    A = eye(m);
    fillRandom(b, seed = 10);

    for 1..iters{
        t.start();
        x = solve_tril(A, b);
        t.stop();
    }

    writeln('LinearAlgebra.solve_tril:', t.elapsed() / iters);

    t.clear();

    for 1..iters{
        t.start();
        x = solve_triu(A, b);
        t.stop();
    }

    writeln('LinearAlgebra.solve_triu:', t.elapsed() / iters);

    t.clear();
}
