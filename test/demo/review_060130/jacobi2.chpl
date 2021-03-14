use history_accumulator;

config var n : int = 3;
config var epsilon : real = 0.01;
config var verbose : bool = true;

var R : domain(2) = {1..n, 1..n};
var BigR : domain(2) = {0..n+1, 0..n+1};

var A : [BigR] real;
var Temp : [R] real;

proc main() {
  [ij in BigR]  A(ij) = 0.0;
  [j in 1..n] A(n+1,j) = 1.0;

  if (verbose) {
    writeln("Initial configuration:");
    writeln(A);
  }

  var iteration = 0, delta : history_real(size=3) = 1.0;

  while (delta > epsilon) {
    [(i,j) in R] Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
    var my_max = 0.0;
    for (i,j) in R {
      my_max = max(my_max, Temp(i,j)-A(i,j));
      A(i,j) = Temp(i,j);
    }
    delta = my_max;
    iteration += 1;
    if (verbose) {
      write("Configuration after iteration: ", iteration);
      writeln(" (delta = ", delta, ")");
      writeln(A);
    }
  }

  writeln("Jacobi computation complete.");
  writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
  writeln("# of iterations: ", iteration);
}
