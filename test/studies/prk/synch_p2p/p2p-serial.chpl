// Chapel's serial implementation of synch_p2p

use PRK;

config const iterations = 1000;
config const m = 1000;
config const n = 1000;
config const debug: bool = false;

param timer: Timer;

if (iterations < 1) {
  writeln("ERROR: iterations must be >= 1 : ", iterations);
  exit(1);
}

if (m < 1 || n < 1) {
  writeln("ERROR: grid dimensions must be positive:", m, ", ", n);
  exit(1);
}

// Initialize and zero out vector
const mrange = 0 .. # m,
      nrange = 0 .. # n,
      Dom = {mrange, nrange},
      Dom1 = {1 ..(m-1), 1 .. (n-1)};

var vector : [Dom] real = 0.0;

writeln("Parallel Research Kernels version ", PRKVERSION);
writeln("Serial pipeline execution on 2D grid");
writeln("Grid sizes                = ", m, ", ", n);
writeln("Number of iterations      = ", iterations);

// Set boundary values (top and left side of grid)
serial do [j in nrange] vector[0,j] = j;
serial do [i in mrange] vector[i,0] = i;

for iteration in 0 .. iterations {

  // Start timer after warmup iteration
  if (iteration == 1) then timer.start();

  for (i,j) in Dom1 {
    vector[i, j] = vector[i-1, j] + vector[i, j-1] - vector[i-1, j-1];
  }

  // Copy bottom right corner value to top left, creating dependency
  vector[0, 0] = -vector[m-1, n-1];
}

timer.stop();

var pipelineTime = timer.elapsed();

// Analyze and output results

// Error threshold
const epsilon = 1.e-8;

// Verify correctness, using bottom right value of array
var cornerValue : real = (iterations + 1)*(n + m - 2);
if (abs(vector[m-1, n-1] - cornerValue) / cornerValue > epsilon) {
  writeln("ERROR: checksum ", vector[m-1, n-1], " does not match verification \
      value", cornerValue);
  exit(1);
}

if (debug) {
  writeln("Solution validates; verification value = ", cornerValue);
} else {
  writeln("Solution validates");
}

var avgTime = pipelineTime / iterations;

writeln("Rate (MFlops/s): ", 1.0e-6 * 2 * ((m-1)*(n-1)) / avgTime,
        " Avg time (s): ", avgTime);
