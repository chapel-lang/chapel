// Configuration variables for program:
config var n = 5,                       // size of nxn grid
           epsilon = 0.00001,           // convergence tolerance
           verbose = false;             // printing control

def main() {
  const ProblemSpace = [1..n, 1..n],    // domain for interior grid points
        BigDomain = [0..n+1, 0..n+1];   // larger domain including boundary points

  var A, Temp: [BigDomain] real = 0.0;  // A contains approximate solution
                                        // Temp is work array used in Jacobi iteration
  A[n+1, 1..n] = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(A, "\n");
  }

  var iteration = 0,                    // iteration counter
      delta: real;                      // measure of convergence at each iteration

  do {
    // compute next approximation using Jacobi method and store in Temp
    forall (i,j) in ProblemSpace do
      Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;

    // compute difference between next and current approximations
    delta = max reduce abs(Temp[ProblemSpace] - A[ProblemSpace]);

    // update A with next approximation
    A[ProblemSpace] = Temp[ProblemSpace];

    // advance iteration counter
    iteration += 1;

    if (verbose) {
      writeln("iteration: ", iteration);
      writeln(A);
      writeln("delta: ", delta, "\n");
    }
  } while (delta > epsilon);

  writeln("Jacobi computation complete.");
  writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
  writeln("# of iterations: ", iteration);
}
