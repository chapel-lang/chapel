config var n : int = 5;
config var epsilon : real = 0.00001;
config var verbose : bool = false;

var R = {1..n, 1..n};
var BigR = R.expand(1);
var South = R.exterior(1, 0);

var A : [BigR] real;
var Temp : [R] real;

forall ij in BigR do
  A(ij) = 0.0;

forall ij in South do
  A(ij) = 1.0;

if (verbose) {
  writeln("Initial configuration:");
  writeln(A);
}

var iteration : int = 0;
var delta : sync real = 1.0;

while (delta > epsilon) {
  forall (i,j) in R do
    Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
  delta = 0.0;
  forall (i,j) in R {
    delta = max(delta.readFE(), Temp(i,j)-A(i,j));
    A(i,j) = Temp(i,j);
  }
  iteration += 1;
  if (verbose) {
    writeln("iteration: ", iteration);
    writeln("delta:     ", delta.readXX());
    writeln(A);
  }
}

writeln("Jacobi computation complete.");
writeln("Delta is ", delta.readXX(), " (< epsilon = ", epsilon, ")");
writeln("# of iterations: ", iteration);
