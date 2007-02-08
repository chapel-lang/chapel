enum classVals {S, W, A, B, C, D, O};


config var probClass = S;


const Class: domain(classVals) = [S..O];
const probSizes:   [Class] = (/ 1400, 7000, 14000, 75000, 150000, 150000, 1400 /),
      nonZeroes:   [Class] = (/ 7, 8, 11, 13, 15, 21, 7 /),
      shifts:      [Class] = (/ 10, 12, 20, 60, 110, 500, 10 /),
      numIters:    [Class] = (/ 15, 15, 15, 75, 75, 100, 15 /),
      verifyZetas: [Class] = (/ 8.5971775078648, 10.362595087124,
                                17.130235054029, 22.712745482631,
                                28.973605592845, 52.5145321058, 0.0 /);


type elemType = real(64);


def main() {
  const n = probSizes(probClass),
        nonzer = nonZeroes(probClass),
        shift = shifts(probClass),
        niter = numIters(probClass),
        rcond = 1.0,
        zetaVerifyValue = verifyZetas(probClass);

  const MatrixSpace = [1..n, 1..n],
        SparseMatSpace: sparse subdomain(MatrixSpace) = ???;

  var A: [SparseMatSpace] elemType = ???;

  const VectorSpace = [1..n];
  var X: [VectorSpace] elemType;

  for trial in 1..numTrials {
    X = 1.0;

    const startTime = getCurrentTime();

    for it in 1..numIter {
      const (rnorm, Z) = conjGrad(A, X);

      const normTemp = (sum reduce (X*Z), 1.0 / sqrt(sum reduce(Z*Z))),
            zeta = shift + 1.0 / normTemp(1);

      if (verbose) then
        writeln(it, rnorm, zeta);

      X = normTemp(2)*Z;
    }

    const runtime = getCurrentTime() - startTime;

    writeln("Execution time = ", runtime);

    if (zetaVerifyValue) {
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


def conjGrad(A: [?MatDom], X: [?VectDom]) {
  const cgitmax = 25;

  var Z: [Vectdom] = 0.0,
      R = X,
      P = R;
  var rho = sum reduce R**2;
 
  for cgit in 1..cgitmax {
    const Q = sum reduce(dim=2) [(i,j) in MatDom] (A(i,j) * P(j));

    const alpha = rho / sum reduce (P*Q);
    Z += alpha*P;
    R -= alpha*Q;

    var rho0 = rho;
    rho = sum reduce R**2;
    var beta = rho / rho0;
    P = R + beta*P;
  }
  R = sum reduce(dim=2) [(i,j) in MatDom] (A(i,j) * Z(j));
  const rnorm = sqrt(sum reduce (X-R)**2);

  return (Z, rnorm);
}
