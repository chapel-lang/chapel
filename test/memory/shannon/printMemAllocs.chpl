use MemDiagnostics;

config var n : int = 20;
config var epsilon : real = 0.00001;
config var verbose : bool = false;

var R : domain(2) = {1..n, 1..n};
var BigR : domain(2) = {0..n+1, 0..n+1};
var South : domain(2) = {n+1..n+1, 1..n};

var A : [BigR] real;
var Temp : [R] real;

var iteration : int = 0;
var delta : real = 1.0;

{ // these curly braces keep compiler-added
  // temporary values from being reported
  // in the below printMemAllocs...
  // printMemAllocs prints entries for
  // whatever is currently allocated.

  forall (i,j) in BigR {
    A(i,j) = 0.0;
  }

  forall (i,j) in South {
    A(i,j) = 1.0;
  }

  if (verbose) {
    writeln("Initial configuration:");
    writeln(A);
  }

  while (delta > epsilon) {
    forall (i,j) in R {
      Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
    }
    delta = 0.0;
    for (i,j) in R {
      delta = max(delta, Temp(i,j)-A(i,j));
      A(i,j) = Temp(i,j);
    }
    iteration += 1;
    if (verbose) {
      writeln("iteration: ", iteration);
      writeln("delta:     ", delta);
      writeln(A);
    }
  }
}
printMemAllocs(1000);

writeln("Jacobi computation complete.");
writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
writeln("# of iterations: ", iteration);
