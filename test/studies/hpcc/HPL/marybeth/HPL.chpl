use Init;
use FactorSolve;
use Time;

config var inputfile = "HPL.dat";

def main() {
  var TEST = HPLparams(inFileName=inputfile);
  var outfile = file(TEST.outFileName, path='./', mode='w');
  outfile.open();

//for p in TEST.P {
//  for q in TEST.Q {
//    Use p, q and pmap to define distribution for A.
//    The right hand side, b, is replicated in each process column.
//    The solution x, is replicated in each process row.
  for n in TEST.N {
    for nb in TEST.NB {
       var ADom = [1..n, 1..n];
       var xDom = [1..n];
       var bDom = [1..n];
       var piv: [1..n] int;
       var A: [ADom] real;
       var x: [xDom] real;
       var b: [bDom] real;
       var resid: 3*real;
       var norms: 5*real;

       var testTimer = Timer();
       var timeData: 2*real; 

       init(A,b);
       testTimer.clear();
       testTimer.start();
       rightBlockLU(A, nb, piv);   
       timeData(1) = testTimer.elapsed();
       LUSolve(A, x, b, piv);
       timeData(2) = testTimer.elapsed();
       testTimer.stop();
       testResults(outfile, n, nb);
       timingResults(outfile, n, timeData);
       init(A,b);
       testSolution(A, x, b, TEST.epsil, resid, norms);
       errorResults(outfile, TEST, resid, norms);
    }
  }
}

def testResults(ofile, n, nb) {
   ofile.writeln("====================================================================");
   ofile.writeln("N = ", n, ", NB = ", nb);
}

def timingResults(ofile, n, timeData) {
   var GFlops = ((n:real/1.0e+9) * (n:real/timeData(2)))*
                ((2.0/3.0) * n:real + (3.0/2.0));
   ofile.writeln("GFlops             = ", GFlops);
   ofile.writeln("Total elapsed time = ", timeData(2));
   ofile.writeln("  Factor time      = ", timeData(1));
   ofile.writeln("  Solve time       = ", (timeData(2) - timeData(1)));
}

def testSolution(A: [?ADom], x: [?xDom], b: [xDom], in eps: real, 
     out resid: 3*real, out norms: 5*real) {
 
  var bHat: [xDom] real;
  var n = ADom.dim(1).length;

  for (i1, i2) in (ADom.dim(1), xDom) {
    for (j1, j2) in (ADom.dim(2), xDom) {
       bHat(i1) += A(i1,j1)*x(j2);
    }
  }

// Look at distributions when implementing these norms.
// Should these norm computations be put into functions?
  var errNorm = max reduce (abs(bHat - b));
  var ANorm1 = max reduce [j in ADom.dim(2)] 
               (+ reduce abs(A[ADom.dim(1), j]));
  var ANormInf = max reduce [i in ADom.dim(1)] 
                 (+ reduce abs(A[i, ADom.dim(2)]));
  var xNorm1 = + reduce abs(x);
  var xNormInf = max reduce abs(x);

  resid(1) = errNorm/(eps*ANorm1*n);
  resid(2) = errNorm/(eps*ANorm1*xNorm1);
  resid(3) = errNorm/(eps*ANormInf*xNormInf*n);

  norms = (errNorm, ANormInf, ANorm1, xNormInf, xNorm1);
}

def errorResults(ofile, TEST, resid: 3*real, norms: 5*real) {
 
  var thresh = TEST.Thresh;
  if (max((...resid)) < thresh) 
    then TEST.kpass += 1; 
    else TEST.kfail += 1;

  ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * N)             = ", resid(1),
     ".....", if (resid(1) < thresh) then "PASSED" else "FAILED");
  ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * ||x||_1)       = ", resid(2),
     ".....", if (resid(2) < thresh) then "PASSED" else "FAILED");
  ofile.writeln("||Ax-b||_oo / (eps * ||A||_oo * ||x||_oo * N) = ", resid(3),
     ".....", if (resid(3) < thresh) then "PASSED" else "FAILED");

  if (max((...resid)) >= thresh) {
    ofile.writeln("||Ax-b||_oo                                   = ", norms(1));
    ofile.writeln("||A||_oo                                      = ", norms(2));
    ofile.writeln("||A||_1                                       = ", norms(3));
    ofile.writeln("||x||_oo                                      = ", norms(4));
    ofile.writeln("||x||_1                                       = ", norms(5));
  }
}
