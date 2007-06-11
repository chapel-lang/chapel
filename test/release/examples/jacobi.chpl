/*
 *  Jacobi Method Example
 *
 *  This program computes the solution of a Laplace equation using 
 *  the Jacobi method.
 *
 */

// Configuration variables for program:
config var n = 5,                       // size of nxn grid
           epsilon = 0.00001,           // convergence tolerance
           verbose = false;             // printing control

def main() {
  const ProblemSpace = [1..n, 1..n],    // domain for interior grid points
        BigDomain = [0..n+1, 0..n+1];   // domain with boundary points

  var X, XNew: [BigDomain] real = 0.0;  // X stores approximate solution
                                        // XNew is work array 
  X[n+1..n+1, 1..n] = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(X, "\n");
  }

  var iteration = 0,                    // iteration counter
      delta: real;                      // measure of convergence 

  do {
    // compute next approximation using Jacobi method and store in XNew
    forall (i,j) in ProblemSpace do
      XNew(i,j) = (X(i-1,j) + X(i+1,j) + X(i,j-1) + X(i,j+1)) / 4.0;

    // compute difference between next and current approximations
    delta = max reduce abs(XNew[ProblemSpace] - X[ProblemSpace]);

    // update X with next approximation
    X[ProblemSpace] = XNew[ProblemSpace];

    // advance iteration counter
    iteration += 1;

    if (verbose) {
      writeln("iteration: ", iteration);
      writeln(X);
      writeln("delta: ", delta, "\n");
    }
  } while (delta > epsilon);

  writeln("Jacobi computation complete.");
  writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
  writeln("# of iterations: ", iteration);
}
