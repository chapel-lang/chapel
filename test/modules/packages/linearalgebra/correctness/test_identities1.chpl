use LinearAlgebra;
use Random;

config const seed=3141592;
config const verbose=false;

// Define some basic numbers
config const L=128, M=217, N=333, P=107;
config const nIters=10;
config const singleThresh=1.0e-5;
config const doubleThresh=1.0e-12; 


test_trace_rotate(real(32));
test_trace_rotate(real(64));



///////////////////////////////////////
/// Test codes go below
///////////////////////////////////////

proc threshold(type t) {
  select t {
    when real(32) do return singleThresh;
    when real(64) do return doubleThresh;
    otherwise halt("Unknown type!");
    }
}

// Relative comparisons -- good when expected != 0
proc testRelCompare(expected, actual, thresh : real,testname:string) {
  var diff = abs(actual/expected - 1.0);
  if diff > thresh {
    writef("Failure in %s : %er \n",testname, diff);
  }
}

// Absolute comparisons
proc testAbsCompare(expected, actual, thresh : real,testname:string) {
  var diff = abs(actual - expected);
  if diff > thresh {
    writef("Failure in %s : %er \n",testname, diff);
  }
}

proc test_trace_rotate(type t) {
  const eps = threshold(t);
  /* tr(AB) = tr(BA) */
  var A = Matrix(M, N, t);
  var B = Matrix(N, M, t);

  var rng=new RandomStream(t, seed=seed);
  for ii in 0.. #nIters {
    rng.fillRandom(A);
    rng.fillRandom(B);
    var t1 = trace(dot(A,B));
    var t2 = trace(dot(B,A));
    testRelCompare(t1,t2,eps,"trace_rotate");
  }
  if verbose {
    writeln("Test trace_rotate passed!");
  }
}


