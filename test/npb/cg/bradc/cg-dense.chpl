use Random;

enum classVals {S, W, A, B, C, D};


config const probClass = S;

const Class: domain(classVals);
const probSizes:   [Class] int = (/ 1400, 7000, 14000, 75000, 150000, 150000 /),
      nonZeroes:   [Class] int = (/ 7, 8, 11, 13, 15, 21 /),
      shifts:      [Class] int = (/ 10, 12, 20, 60, 110, 500 /),
      numIters:    [Class] int = (/ 15, 15, 15, 75, 75, 100 /),
      verifyZetas: [Class] real = (/ 8.5971775078648, 10.362595087124,
                                    17.130235054029,  22.712745482631,
                                    28.973605592845,  52.5145321058 /);

config const n = probSizes(probClass),
             rcond = 0.1,
             nonzer = nonZeroes(probClass),
             shift = shifts(probClass),
             niter = numIters(probClass),
             zetaVerifyValue = verifyZetas(probClass);

type elemType = real(64);

config const numTrials = 1,
             verbose = true,
             debug = false,
             printTiming = false;


def main() {
  const MatrixSpace = [1..n, 1..n];
  var A: [MatrixSpace] elemType;

  for ((r,c), v) in makea() {
    A(r,c) += v;
  }

  const VectorSpace = [1..n];
  var X: [VectorSpace] elemType,
      zeta = 0.0;

  for trial in 1..numTrials {
    X = 1.0;

    const startTime = getCurrentTime();

    for it in 1..niter {
      const (Z, rnorm) = conjGrad(A, X);

      const normTemp1 = + reduce (X*Z);
      const normTemp2 = 1.0 / sqrt(+ reduce(Z*Z));
      if debug then writeln("normTemp2 is: ", normTemp2);
      zeta = shift + 1.0 / normTemp1;

      if (verbose) then
        writeln(it, " ", rnorm, " ", zeta);

      X = normTemp2*Z;
    }

    const runtime = getCurrentTime() - startTime;

    if printTiming then writeln("Execution time = ", runtime);

    if (zetaVerifyValue != 0.0) {  // BLC: aww, have to compare against 0.0
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


def conjGrad(A: [?MatDom], X: [?VectDom]) {
  const cgitmax = 25;

  var Z: [VectDom] real = 0.0,
      R = X,
      P = R;
  var rho = + reduce R**2;
 
  if debug then writeln("P is: ", P);

  for cgit in 1..cgitmax {
    // WANT (a partial reduction):
    //    const Q = + reduce(dim=2) [(i,j) in MatDom] (A(i,j) * P(j));
    // INSTEAD OF:
    const Q: [VectDom] real;
    [i in MatDom(1)] Q(i) = + reduce [j in MatDom(2)] (A(i,j) * P(j));
    //

    const alpha = rho / + reduce (P*Q);
    Z += alpha*P;
    R -= alpha*Q;

    var rho0 = rho;
    rho = + reduce R**2;
    var beta = rho / rho0;
    P = R + beta*P;
  }
  // WANT (a partial reduction):
  //      R = + reduce(dim=2) [(i,j) in MatDom] (A(i,j) * Z(j));
  // INSTEAD OF:
  [i in MatDom(1)] R(i) = + reduce [j in MatDom(2)] (A(i,j) * Z(j));
  //

  if debug then writeln("X is: ", X);
  if debug then writeln("R is: ", R);

  const rnorm = sqrt(+ reduce ((X-R)**2));
  if debug then writeln("rnorm is: ", rnorm);

  if debug then writeln("Z is: ", Z);

  return (Z, rnorm);
}


iterator makea() {
  var v: [1..nonzer+1] real,    // BLC: insert domains? or grow as necessary?
      iv: [1..nonzer+1] int;
  
  var size = 1.0;
  const ratio = rcond ** (1.0 / n);

  var randStr = RandomStream(314159265);

  for iouter in 1..n {
    var nzv = nonzer;

    sprnvc(n, nzv, v, iv, randStr);
    vecset(v, iv, nzv, iouter, 0.50);

    // BLC: replace with zippered loop over iv or iv(1..nzv)?
    for ivelt in 1..nzv {
      const jcol = iv(ivelt),
            scale = size * v(ivelt);

      // BLC: replace with zippered loop over iv or iv(1..nzv)?
      for ivelt1 in 1..nzv {
        const irow = iv(ivelt1);

        yield ((irow, jcol), v(ivelt1)*scale);
      }
    }
    size *= ratio;
  }

  for i in 1..n {
    yield ((i, i), rcond - shift);
  }
}


def sprnvc(n, nz, v, iv, randStr) {
  var nn1 = 1;
  while (nn1 < n) do nn1 *= 2;

  var indices: domain(int);

  for nzv in 1..nz {
    var vecelt: real, i: int;
    do {
      vecelt = randStr.getNext();
      var vecloc = randStr.getNext(); 
      i = (vecloc * nn1):int + 1;
    } while (i > n || indices.member?(i));
    indices += i;
    v(nzv) = vecelt;
    iv(nzv) = i;
  }
}


def vecset(v, iv, inout nzv, i, val) {
  var set = false;
  for k in 1..nzv {
    if (iv(k) == i) {
      v(k) = val;
      set = true;
    }
  }
  if (!set) {
    nzv += 1;
    v(nzv) = val;
    iv(nzv) = i;
  }
}
