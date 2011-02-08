// configuration constants
config const n = 10;              // problem size
config const epsilon = 0.0001;    // threshold for iteration

const D: domain(2) = [1..n, 1..n];



param StencilD: domain(2) = [-1..1, -1..1];

param weights: [StencilD] real = ( ( 0.00, 0.25, 0.00 ),
                                   ( 0.25, 0.00, 0.25 ),
                                   ( 0.00, 0.25, 0.00 ) );


proc main() {
  var A, B: [D] real = 0.0;

  A.setBoundary(computeBoundary);



  // it's a shame not to be able to declare this when it's used
  var bigdiff: real;

  do {
    // the hope is that some major constant folding/stencil
    // optimization will take place here
    [ij in D] B(ij) = sum reduce [off in StencilD] A(ij+off);

    bigdiff = max reduce abs(A-B);

    A = B;
  } while (bigdiff > epsilon);

  writeln("Final A is: \n", A);
}


proc computeBoundary((i, j): index(2)) {
  if (i < 0 || i > n+1 || j < 0 || j > n+1) {
    halt("Out of bounds error in computeBoundary");
  } else if (j == n+1) {
    return 1.0;
  } else {
    return 0.0;
  }
}


// Here, we posit the existinence of an "out-of-bounds" function used
// to set array boundary conditions rather than declaring them
// explicitly.  My claim is that support for this shouldn't be any
// harder than doing ghost cells well.  This support allows:
//   * explicit boundary conditions to be removed from the code
//   * A and B to be declared to be the same size
//   * the reduction, array copy back, and output of A to be simpler

