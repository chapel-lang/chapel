use BlockDist;

config var n : int = 5;
config var epsilon : real = 0.00001;
config var verbose : bool = false;

const Dist = new dmap(new Block({1..n,1..n},dataParTasksPerLocale=1));

var R : domain(2) dmapped Dist = {1..n, 1..n};
writeln(R);
var BigR : domain(2) dmapped Dist = {0..n+1, 0..n+1};
var South : domain(2) dmapped Dist = {n+1..n+1, 1..n};

var A : [BigR] real;
writeln(A);

var Temp : [R] real;

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

var iteration : int = 0;
var delta : real = 1.0;

while (delta > epsilon) {
  forall (i,j) in R {
    Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
  }
  delta = + reduce [(i,j) in R] Temp(i,j) - A(i,j);
  forall (i,j) in R {
    A(i,j) = Temp(i,j);
  }
  iteration += 1;
  if (verbose) {
    writeln("iteration: ", iteration);
    writeln("delta:     ", delta);
    writeln(A);
  }
}

writeln("Jacobi computation complete.");
writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
writeln("# of iterations: ", iteration);
