// Implementation of Jacobi transform

var n: int = 6;
var Full: domain(2) = {1..n,1..n};
var Inner: subdomain(Full) = {2..n-1,2..n-1};
var A: [Full] real, B: [Full] real;
var threshold: real = 0.00001;

proc main() {
  initialize();
  while (true) {
    if (stencil(A,B) < threshold) {
      writeln(B);
      break;
    }
    if (stencil(B,A)  < threshold) {
      writeln(A);
      break;
    }
  }
}

// Put the stencil of X in Y
// Return the max difference between the new and old stencils
proc stencil(X,Y) {
  forall (i,j) in Inner {
    Y(i,j) = (X(i+1,j) + X(i-1,j) + X(i,j+1) + X(i,j-1)) / 4;
  }
  return max reduce [i in Inner] abs(X(i) - Y(i));
}

// Initialize everything but the south boundary to 0.0
// Initialize the south boundary to 1.0
proc initialize() {
  forall i in {n..n,1..n} {
    A(i) = 1.0;
    B(i) = 1.0;
  }
}
