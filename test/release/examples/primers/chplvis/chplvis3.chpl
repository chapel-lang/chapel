// chplvis: Jacobi Iterations Example

// Example 3 -- A more complex chplvis example
// A version of Jacobi Iterations written to run on multiple locales

use VisualDebug;
use BlockDist;

// Typical config variables
config var n : int = 10;
config var epsilon : real = 0.0001;
config var verbose : bool = false;

// Allow multiple computation steps before doing a reduction
// Jacobi will not diverge, so further computation just converges more
config var compLoop : int = 1;

// Allow different runs to create different data directories so it is
// easier to compare runs with chplvis.
config var dirname = "E3";

// Start VisualDebug here to see that distributed domain and variable
// declarations generate tasks and communication.
startVdebug(dirname);

const space = {1..n, 1..n};
const bspace = {0..n+1, 0..n+1};
const bsouth = {n+1..n+1, 1..n};

// Dmapped versions of the domains
const R = space dmapped Block(boundingBox=bspace);
const BigR = bspace dmapped Block(boundingBox=bspace);
const South = bsouth dmapped Block(boundingBox=bspace);

// Dmapped arrays
var A : [BigR] real;
var Temp : [R] real;
var Diff   : [R] real;

tagVdebug("init phase");

// Initialize Temp because the computation step
// copies Temp and then computes a new Temp

forall (i,j) in BigR {
  A(i,j) = 0.0;
}

forall (i,j) in R {
  Temp(i,j) = 0.0;
}

tagVdebug("boundary");

forall (i,j) in South {
  A(i,j) = 1.0;
}

pauseVdebug();

if (verbose) {
  writeln("Initial configuration:");
  writeln(A);
}

var iteration : int = 0;
var delta : real = 1;

// Main computation loop -- we want to see the two parts of this
// loop, the computation and the reduction part.

while (delta > epsilon) {

  // Tag the computation part of this loop 
  tagVdebug("computation");

  for t in 1 .. compLoop do {
    forall (i,j) in R do
      A(i,j) = Temp(i,j);
    forall (i,j) in R do
      Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
  }
  
  // tag the reduction part of this loop.
  tagVdebug("max");
  forall (i,j) in R {
    Diff(i,j) = abs(Temp(i,j)-A(i,j));
  }
  delta = max reduce Diff;

  pauseVdebug();
  iteration += compLoop;
  if (verbose) {
    writeln("iteration: ", iteration);
    writeln("delta:     ", delta);
    writeln(Temp);
  }
}

stopVdebug();

writeln("Jacobi computation complete.");
writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
writeln("# of iterations: ", iteration);
