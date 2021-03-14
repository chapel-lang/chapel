use Init;
use FactorSolve;
use Time;
use IO;

config var inputfile = "HPL.dat";
config var writeTimingInfo = false;
config var writeAccuracyInfo = false;

proc main() {
  var TEST = new HPLparams(inFileName=inputfile);
  var outfile = open(TEST.outFileName, iomode.cw).writer();

//for p in TEST.P {
//  for q in TEST.Q {
//    A = [Asquare | b].  All of A is factored, so that only
//    a backsolve is needed to solve for x.  (The forward solve
//    step is implicitly done in the factor step.)

//    Use p, q and pmap to define distribution for A.
//    The right hand side, b, is replicated in each process column.
//    The solution x, is replicated in each process row.
  for n in TEST.N {
    for nb in TEST.NB {
       var ADom = {1..n, 1..n+1};
       var xDom = {1..n};
       var A: [ADom] real;
       var x: [xDom] real;
       var resid: 3*real;
       var norms: 5*real;

       var testTimer = new Timer();
       var timeData: 2*real; 

//     A is initialized to be a (n x n+1) matrix, since
//     the right hand side is stored with the matrix.
       init(A);

//     Clear and start the timers.
       testTimer.clear();
       testTimer.start();

//     Factor A = [Asquare | b].
       rightBlockLU(A, nb);   
       timeData(0) = testTimer.elapsed();

//     Solve for x.
       LUSolve(A, x);
       timeData(1) = testTimer.elapsed();
       testTimer.stop();

//     Write info about current test.
       testResults(outfile, n, nb);

//     Write timing info, if specified.
       if writeTimingInfo then timingResults(outfile, n, timeData);

//     Reinitialize A and perform accuracy tests.
       init(A);
       testSolution(A, x, TEST.epsil, resid, norms);

//     Write error information.
       errorResults(outfile, TEST, resid, norms);
    }
  }

  outfile.close();

}

proc testResults(ofile, n, nb) {
   ofile.writeln("====================================================================");
   ofile.writeln("N = ", n, ", NB = ", nb);
}

proc timingResults(ofile, n, timeData) {
   var GFlops = ((n:real/1.0e+9) * (n:real/timeData(1)))*
                ((2.0/3.0) * n:real + (3.0/2.0));
   ofile.writeln("GFlops             = ", GFlops);
   ofile.writeln("Total elapsed time = ", timeData(1));
   ofile.writeln("  Factor time      = ", timeData(0));
   ofile.writeln("  Solve time       = ", (timeData(1) - timeData(0)));
}

proc testSolution(A: [?ADom], x: [?xDom], in eps: real, 
     out resid: 3*real, out norms: 5*real) {
 
  var n = ADom.dim(0).size;
  ref b = A(..,n+1);
  var bHat: [xDom] real;

  for (i1, i2) in zip(ADom.dim(0), xDom) {
    for (j1, j2) in zip(ADom.dim(0), xDom) {
       bHat(i1) += A(i1,j1)*x(j2);
    }
  }

// Look at distributions when implementing these norms.
// Should these norm computations be put into functions?
  var errNorm = max reduce (abs(bHat - b));
  var ANorm1 = max reduce [j in ADom.dim(0)] 
               (+ reduce abs(A[ADom.dim(0), j]));
  var ANormInf = max reduce [i in ADom.dim(0)] 
                 (+ reduce abs(A[i, ADom.dim(0)]));
  var xNorm1 = + reduce abs(x);
  var xNormInf = max reduce abs(x);

  resid(0) = errNorm/(eps*ANorm1*n);
  resid(1) = errNorm/(eps*ANorm1*xNorm1);
  resid(2) = errNorm/(eps*ANormInf*xNormInf*n);

  norms = (errNorm, ANormInf, ANorm1, xNormInf, xNorm1);
}

proc errorResults(ofile, in TEST, resid: 3*real, norms: 5*real) {
 
  var thresh = TEST.Thresh;
  if (max((...resid)) < thresh) 
    then TEST.kpass += 1; 
    else TEST.kfail += 1;

  if writeAccuracyInfo {
    ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * N)             = ", resid(0),
     ".....", if (resid(0) < thresh) then "PASSED" else "FAILED");
    ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * ||x||_1)       = ", resid(1),
     ".....", if (resid(1) < thresh) then "PASSED" else "FAILED");
    ofile.writeln("||Ax-b||_oo / (eps * ||A||_oo * ||x||_oo * N) = ", resid(2),
     ".....", if (resid(2) < thresh) then "PASSED" else "FAILED");

    ofile.writeln("||Ax-b||_oo                                   = ", norms(0));
    ofile.writeln("||A||_oo                                      = ", norms(1));
    ofile.writeln("||A||_1                                       = ", norms(2));
    ofile.writeln("||x||_oo                                      = ", norms(3));
    ofile.writeln("||x||_1                                       = ", norms(4));
  } else {
    ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * N)              ",
     ".....", if (resid(0) < thresh) then "PASSED" else "FAILED");
    ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * ||x||_1)        ",
     ".....", if (resid(1) < thresh) then "PASSED" else "FAILED");
    ofile.writeln("||Ax-b||_oo / (eps * ||A||_1 * N)              ", 
     ".....", if (resid(2) < thresh) then "PASSED" else "FAILED");
  }
}
