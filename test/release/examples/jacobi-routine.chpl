/*
 *  Jacobi Method Example - version 2
 *
 *  This program computes the solution of a Laplace equation using Jacobi method.
 *  This version implements the Jacobi method in a routine.
 *
 */
// Configuration variables for program:
config var n = 5,                                   // size of nxn grid
           epsilon = 0.00001,                       // convergence tolerance
           verbose = false;                         // printing control

def main() {
  const ProblemSpace = [1..n, 1..n];                // domain for interior grid points
  var X: [ProblemSpace] real = 0.0;                 // X contains approx solution

  const delta: real,                                // measure of convergence
        numIterations: int;                         // iteration counter

  (delta, numIterations) = jacobi(X);

  writeln("Jacobi computation complete.");
  writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
  writeln("# of iterations: ", numIterations);
}


def jacobi(X: [?XDomain] real) {
  const BigDomain = XDomain.expand(1);              // Bordered domain
  var XNew: [BigDomain] real;                       // Work array needed for Jacobi
  const highRow = XDomain(1).high;

  XNew[highRow+1, XDomain(2)] = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(X, "\n");
  }

  var iteration = 0,
      delta: real;

  do {
    XNew[XDomain] = X;

    forall (i,j) in XDomain do
      X(i,j) = (XNew(i-1,j) + XNew(i+1,j) + XNew(i,j-1) + XNew(i,j+1)) / 4.0;

    delta = max reduce abs(XNew[XDomain] - X);

    iteration += 1;

    if (verbose) {
      writeln("iteration: ", iteration);
      writeln(X);
      writeln("delta: ", delta, "\n");
    }
  } while (delta > epsilon);

  return (delta, iteration);
}
