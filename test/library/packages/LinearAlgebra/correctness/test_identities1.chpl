use LinearAlgebra;
use Random;
use IO;

config const seed=3141592;
config const verbose=false;

// Define some basic numbers
config const L=128, M=217, N=333, P=107;
config const nIters=10;
config const singleThresh=1.0e-5;
config const doubleThresh=1.0e-14; 

/* A(BC) = (AB)C */
test_assoc(real(32));
test_assoc(real(64));
test_assoc(complex(64));
test_assoc(complex(128));

/* (AB)^t = B^t A^t */
test_transpose_product(real(32));
test_transpose_product(real(64));
test_transpose_product(complex(64));
test_transpose_product(complex(128));

/* tr(AB) = tr(BA) */
test_trace_rotate(real(32));
test_trace_rotate(real(64));
test_trace_rotate(complex(64));
test_trace_rotate(complex(128));

/* vAw = tr(Awv) */
test_trace_vector_rotate(real(32));
test_trace_vector_rotate(real(64));
test_trace_vector_rotate(complex(64));
test_trace_vector_rotate(complex(128));



///////////////////////////////////////
/// Test codes go below
///////////////////////////////////////

/* A(BC) = (AB)C */
// Do an elementwise comparison
proc test_assoc(type t) {
  const testName="assoc [%s]".format(t:string);
  const eps = threshold(t);
  var A = Matrix(L, M, t);
  var B = Matrix(M, N, t);
  var C = Matrix(N, P, t);

  var rng=new owned RandomStream(t, seed=seed);
  for ii in 0.. #nIters {
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    var r1 = dot(A,dot(B,C));
    var r2 = dot(dot(A,B),C);
    var t1 = max reduce abs(r1/r2); 
    if verbose && (ii==0) {
      writef("%s : %n, %n\n",testName, t1, t1-1.0);
    }
    testRelCompare(t1,1.0,eps,testName);
  }
  if verbose {
    writef("Test %s passed!\n", testName);
  }
}

/* (AB)^t = B^t A^t */
// Do an elementwise comparison
proc test_transpose_product(type t) {
  const testName="transpose_product [%s]".format(t:string);
  const eps = threshold(t);
  var A = Matrix(L, M, t);
  var B = Matrix(M, N, t);

  var rng=new owned RandomStream(t, seed=seed);
  for ii in 0.. #nIters {
    rng.fillRandom(A);
    rng.fillRandom(B);
    var r1 = transpose(dot(A,B));
    var r2 = dot(transpose(B),transpose(A));
    var t1 = max reduce abs(r1/r2); 
    if verbose && (ii==0) {
      writef("%s : %n, %n\n",testName, t1, t1-1.0);
    }
    testRelCompare(t1,1.0,eps,testName);
  }
  if verbose {
    writef("Test %s passed!\n", testName);
  }
}

/* tr(AB) = tr(BA) */
proc test_trace_rotate(type t) {
  const testName="trace_rotate [%s]".format(t:string);
  const eps = threshold(t);
  var A = Matrix(M, N, t);
  var B = Matrix(N, M, t);

  var rng=new owned RandomStream(t, seed=seed);
  for ii in 0.. #nIters {
    rng.fillRandom(A);
    rng.fillRandom(B);
    var t1 = trace(dot(A,B));
    var t2 = trace(dot(B,A));
    if verbose && (ii==0) {
      writef("%s : %n, %n\n",testName, t1,t2);
    }
    testRelCompare(t1,t2,eps,testName);
  }
  if verbose {
    writef("Test %s passed!\n", testName);
  }
}

/* vAw = tr(Awv) */
proc test_trace_vector_rotate(type t) {
  const testName="trace_vector_rotate [%s]".format(t:string);
  const eps = threshold(t);
  var A = Matrix(M, M, t);
  var v = Vector(M, t);
  var w = Vector(M, t);

  var rng=new owned RandomStream(t, seed=seed);
  for ii in 0.. #nIters {
    rng.fillRandom(A);
    rng.fillRandom(v);
    rng.fillRandom(w);
    var t1 = dot(dot(A,w),v);
    var t2 = trace(dot(A, outer(w,v)));
    if verbose && (ii==0) {
      writef("%s : %n, %n\n", testName, t1,t2);
    }
    testRelCompare(t1,t2,eps,testName);
  }
  if verbose {
    writef("Test %s passed!\n", testName);
  }
}

//// Helpers
proc threshold(type t) {
  select t {
    when real(32) do return singleThresh;
    when real(64) do return doubleThresh;
    when complex(64) do return singleThresh;
    when complex(128) do return doubleThresh;
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
