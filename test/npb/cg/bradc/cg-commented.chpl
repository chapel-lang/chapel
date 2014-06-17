//
// a separate Chapel module that defines A's structure
//
use CGMakeA;

//
// name for the numerical element type to be used for matrices and vectors
//
type elemType = real(64);

//
// NAS CG defines 2 test problem sizes (S and W) and 4 "real" (A, B, C, D)
//
enum classVals {S, W, A, B, C, D};


//
// An enumerated domain, indexable by elements of the classVals enumeration
//
const Class: domain(classVals);

//
// Problem parameters for each of the problem sizes
//
const probSizes:   [Class] int = ( 1400, 7000, 14000, 75000, 150000, 150000 ),
      nonZeroes:   [Class] int = ( 7, 8, 11, 13, 15, 21 ),
      shifts:      [Class] int = ( 10, 12, 20, 60, 110, 500 ),
      numIters:    [Class] int = ( 15, 15, 15, 75, 75, 100 ),
      verifyZetas: [Class] elemType = ( 8.5971775078648, 10.362595087124,
                                        17.130235054029,  22.712745482631,
                                        28.973605592845,  52.5145321058 );

//
// the problem class for this run (S by default, override on command-line)
//
config const probClass = classVals.S;

//
// the problem parameters defined using the class by default, overridable
//
config const n = probSizes(probClass),
             nonzer = nonZeroes(probClass),
             shift = shifts(probClass),
             niter = numIters(probClass),
             zetaVerifyValue = verifyZetas(probClass);

//
// some experimental parameters
//
config const numTrials = 1,
             verbose = true,
             debug = false,
             printTiming = false;


//
// entry point for the program
//
proc main() {
  //
  // the dense domain defining the bounding box for the matrix
  //
  const DenseSpace = {1..n, 1..n};

  //
  // the sparse subdomain defining the sparse matrix's indices
  //
  var MatrixSpace: sparse subdomain(DenseSpace);

  //
  // the sparse matrix itself
  //
  var A: [MatrixSpace] elemType;

  //
  // makea() is an iterator that yields a series of (row,col) indices
  // along with the numerical array value to be accumulated into that
  // position of the sparse array.  This loop implements that by
  // adding the index to the sparse domain and then accumulating into
  // the array at that location.  This is probably slow due to the
  // array reallocations required, but I use it here because I think
  // it's the most instructive way to write it
  //
  for (ind, v) in makea(elemType, n, nonzer, shift) {
    MatrixSpace += ind;
    A(ind) += v;
  }

  //
  // the (dense) vector sparse
  //
  const VectorSpace = {1..n};

  //
  // our working vector, X, and zeta, which is used for verification
  //
  var X: [VectorSpace] elemType,
      zeta = 0.0;

  //
  // the number of trials to perform the computation (to amortize the
  // cost of building the sparse domain/array over many runs)
  //
  for trial in 1..numTrials {
    //
    // initialize the vector to 1.0 in all locations
    //
    X = 1.0;

    //
    // check the time
    //
    const startTime = getCurrentTime();

    //
    // run for the specified number of iterations
    //
    for it in 1..niter {
      //
      // call the conjugant gradient routine which returns a vector Z
      // and a norm value, rnorm
      //
      const (Z, rnorm) = conjGrad(A, X);

      //
      // compute a new zeta value using a reduction
      //
      zeta = shift + 1.0 / + reduce (X*Z);

      //
      // debugging information for comparison against the NAS version
      //
      if verbose then writeln(it, " ", rnorm, " ", zeta);

      //
      // compute the next value of X
      //
      X = (1.0 / sqrt(+ reduce(Z*Z))) * Z;
    }

    //
    // check the time again
    //
    const runtime = getCurrentTime() - startTime;

    if printTiming then writeln("Execution time = ", runtime);

    //
    // verify the computation
    //
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


//
// computes the conjugant gradient; takes sparse matrix A and vector X
// and queries their respective domains storing as MatDom and VectDom
//
proc conjGrad(A: [?MatDom], X: [?VectDom]) {
  //
  // number of iterations to run
  //
  const cgitmax = 25;

  //
  // three temporary vectors, Z initialized to 0.0 and R, P initialized to X
  //
  var Z: [VectDom] elemType = 0.0,
      R = X,
      P = R;

  //
  // rho, a norm value
  //
  var rho = + reduce R**2;

  for cgit in 1..cgitmax {
    //
    // compute the sparse matrix-vector product by iterating over A's
    // domain, performing the elementwise multiplications, and
    // collapsing the second dimension using a partial reduction,
    // assigning to vector Q
    //
    const Q = + reduce(dim=2) [(i,j) in MatDom] (A(i,j) * P(j));

    //
    // compute alpha and update Z and R accordingly
    //
    const alpha = rho / + reduce (P*Q);
    Z += alpha*P;
    R -= alpha*Q;

    //
    // compute a new rho value and beta and update P accordingly
    //
    const rho0 = rho;
    rho = + reduce R**2;
    const beta = rho / rho0;
    P = R + beta*P;
  }
  //
  // one final sparse matrix-vector multiplication, assigning to R
  //
  R = + reduce(dim=2) [(i,j) in MatDom] (A(i,j) * Z(j));

  //
  // compute the norm
  //
  const rnorm = sqrt(+ reduce ((X-R)**2));

  //
  // return the tuple of results: Z and rnorm
  //
  return (Z, rnorm);
}
