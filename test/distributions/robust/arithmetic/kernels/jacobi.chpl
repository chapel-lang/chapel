/*
 *  Jacobi Method Example
 *
 *  This program computes the solution of a Laplace equation using 
 *  the Jacobi method.
 *
 */

use driver_domains;

// Configuration variables for program:
config var epsilon = 0.00001,           // convergence tolerance
           verbose = false;             // printing control

proc main() {
  const ProblemSpace = Dom2D,           // domain for grid points
    BigDomain = ProblemSpace.expand(1); // domain including boundary points

  var X, XNew: [BigDomain] real = 0.0;  // declare arrays: 
                                        //   X stores approximate solution
                                        //   XNew stores the next solution

  [ij in ProblemSpace.exterior(1,0)]    // Set south boundary values to 1.0
    X[ij] = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(X, "\n");
  }

  var iteration = 0,                    // iteration counter
      delta: real;                      // measure of convergence 

  const north = (-1,0), south = (1,0), east = (0,1), west = (0,-1);

  do {
    // compute next approximation using Jacobi method and store in XNew
    forall ij in ProblemSpace do
      XNew(ij) = (X(ij+north) + X(ij+south) + X(ij+east) + X(ij+west)) / 4.0;

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
