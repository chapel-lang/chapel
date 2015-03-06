use CGMakeA, Time;

type elemType = real(64);

enum classVals {S, W, A, B, C, D};


const Class: domain(classVals);
const probSizes:   [Class] int = ( 1400, 7000, 14000, 75000, 150000, 150000 ),
      nonZeroes:   [Class] int = ( 7, 8, 11, 13, 15, 21 ),
      shifts:      [Class] int = ( 10, 12, 20, 60, 110, 500 ),
      numIters:    [Class] int = ( 15, 15, 15, 75, 75, 100 ),
      verifyZetas: [Class] elemType = ( 8.5971775078648, 10.362595087124,
                                        17.130235054029,  22.712745482631,
                                        28.973605592845,  52.5145321058 );

config const probClass = classVals.S;

config const n = probSizes(probClass),
             nonzer = nonZeroes(probClass),
             shift = shifts(probClass),
             niter = numIters(probClass),
             zetaVerifyValue = verifyZetas(probClass);

config const numTrials = 1,
             verbose = true,
             debug = false,
             printTiming = false;


proc main() {
  const MatrixSpace = {1..n, 1..n};
  var A: [MatrixSpace] elemType;
  // WANT the above to be:
  //     const DenseSpace = {1..n, 1..n};
  //     var MatrixSpace: sparse subdomain(DenseSpace) dist(CSR);
  //     var A: [MatrixSpace] elemType;

  for (ind, v) in makea(elemType, n, nonzer, shift) {
    A(ind) += v;
  }
  // WANT the body of the above to be:
  //     MatrixSpace += ind;
  //     A(ind) += v;

  const VectorSpace = {1..n};
  var X: [VectorSpace] elemType,
      zeta = 0.0;

  for trial in 1..numTrials {
    X = 1.0;

    const startTime = getCurrentTime();

    for it in 1..niter {
      const (Z, rnorm) = conjGrad(A, X);

      zeta = shift + 1.0 / + reduce (X*Z);

      if verbose then writeln(it, " ", rnorm, " ", zeta);

      X = (1.0 / sqrt(+ reduce(Z*Z))) * Z;
    }

    const runtime = getCurrentTime() - startTime;

    if printTiming then writeln("Execution time = ", runtime);

    if (zetaVerifyValue != 0.0) {
      const epsilon = 1.0e-10;
      if (abs(zeta - zetaVerifyValue) <= epsilon) {
        writeln("Verification successful");
      } else {
        writeln("Verification failed");
      }
    } else {
      writeln("No verification performed");
    }
  }
}


proc conjGrad(A: [?MatDom], X: [?VectDom]) {
  const cgitmax = 25;

  var Z: [VectDom] elemType = 0.0,
      R = X,
      P = R;
  var rho = + reduce R**2;
 
  for cgit in 1..cgitmax {
    // WANT (a partial reduction):
    //    const Q = + reduce(dim=2) [(i,j) in MatDom] (A(i,j) * P(j));
    // INSTEAD OF:
    var Q: [VectDom] elemType;
    [i in MatDom.dim(1)] Q(i) = + reduce [j in MatDom.dim(2)] (A(i,j) * P(j));
    //

    const alpha = rho / + reduce (P*Q);
    Z += alpha*P;
    R -= alpha*Q;

    const rho0 = rho;
    rho = + reduce R**2;
    const beta = rho / rho0;
    P = R + beta*P;
  }
  // WANT (a partial reduction):
  //      R = + reduce(dim=2) [(i,j) in MatDom] (A(i,j) * Z(j));
  // INSTEAD OF:
  [i in MatDom.dim(1)] R(i) = + reduce [j in MatDom.dim(2)] (A(i,j) * Z(j));
  //

  const rnorm = sqrt(+ reduce ((X-R)**2));

  return (Z, rnorm);
}
