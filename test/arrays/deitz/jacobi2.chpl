config var n : integer = 5;
config var epsilon : float = 0.00001;
config var verbose : boolean = false;

var R : domain(2) = (1..n, 1..n);
var BigR : domain(2) = (0..n+1, 0..n+1);

var A : [BigR] float;
var Temp : [R] float;

[i,j in BigR] A(i,j) = 0.0;

[i,j in (n+1..n+1, 1..n)] A(i,j) = 1.0;

if (verbose) {
  writeln("Initial configuration:");
  writeln(A);
}

var iteration : integer = 0;
var delta : float = 1.0;

while (delta > epsilon) {
  [i,j in R] Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
  delta = 0.0;
  [i,j in R] {
    delta += Temp(i,j)-A(i,j);
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
