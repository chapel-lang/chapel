config var n = 5,
           epsilon = 0.00001,
           verbose = false;

def main() {
  const ProblemSpace = [1..n, 1..n],
        BigDomain = [0..n+1, 0..n+1];

  var A, Temp: [BigDomain] real = 0.0;

  [j in 1..n] A(n+1, j) = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(A, "\n");
  }

  var iteration = 0,
      delta: real;

  do {
    forall (i,j) in ProblemSpace do
      Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;

    delta = max reduce [ij in ProblemSpace] abs(Temp(ij) - A(ij));
    A(ProblemSpace) = Temp(ProblemSpace);

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
