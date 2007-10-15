use Init;
use FactorSolve;

config var inputfile = "HPL.dat";

def main() {
  var TEST = HPLparams(inFileName="HPL.dat");

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

       init(A,b);
       rightBlockLU(A, nb, piv);   
       LUSolve(A, x, b, piv);
       init(A,b);
       testSolution(A, x, b, TEST.epsil, resid);
       writeln("Solving system of size n = ", n);
       writeln("Blocksize = ", nb);
       writeln("Computed residual errors = ", resid);
    }
  }
}

def testSolution(A: [?ADom], x: [?xDom], b: [xDom], in eps: real, out resid: 3*real) {
 
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
}
