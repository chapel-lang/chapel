use Init;
use FactorSolve;

config var inputfile = "HPL.dat";

def main() {
  var paramList = HPLparams(inFileName="HPL.dat");

  for n in paramList.N {
    for nb in paramList.NB {
       var ADom = [1..n, 1..n];
       var xDom = [1..n];
       var piv: [1..n] int;
       var A: [ADom] real;
       var x: [xDom] real;
       var b: [xDom] real;
       init(A,b);
       rightBlockLU(A, nb, piv);   
       x = 1;
       LUSolve(A, x, b, piv);
       writeln("x = ", x);
       testSolution(A, x, b, paramList.Thresh);
    }
  }
}

def testSolution(A: [?ADom], x: [?xDom], b: [xDom], thresh) {
 
  var bHat: [xDom] real;

  for (i1, i2) in (ADom.dim(1), xDom) {
    for (j1, j2) in (ADom.dim(2), xDom) {
       bHat(i1) += A(i1,j1)*x(j2);
    }
  }

  writeln('A*x = ', bHat);
  writeln('b = ', b);
  var errNorm = max reduce (abs(bHat - b));

  writeln("Computed residual error = ", errNorm);

}
