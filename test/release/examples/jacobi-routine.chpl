config var n = 5,
           epsilon = 0.00001,
           verbose = false;

def main() {
  const ProblemSpace = [1..n, 1..n];
  var A: [ProblemSpace] real = 0.0;

  const delta: real,
        numIterations: int;

  (delta, numIterations) = jacobi(A);

  writeln("Jacobi computation complete.");
  writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
  writeln("# of iterations: ", numIterations);
}


def jacobi(A: [?ADomain] real) {
  const BigDomain = ADomain.expand(1);
  var Temp: [BigDomain] real;
  const highRow = ADomain(1).high;

  Temp[highRow+1, ADomain(2)] = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(A, "\n");
  }

  var iteration = 0,
      delta: real;

  do {
    Temp[ADomain] = A;

    forall (i,j) in ADomain do
      A(i,j) = (Temp(i-1,j) + Temp(i+1,j) + Temp(i,j-1) + Temp(i,j+1)) / 4.0;

    delta = max reduce abs(Temp[ADomain] - A);

    iteration += 1;

    if (verbose) {
      writeln("iteration: ", iteration);
      writeln(A);
      writeln("delta: ", delta, "\n");
    }
  } while (delta > epsilon);

  return (delta, iteration);
}
