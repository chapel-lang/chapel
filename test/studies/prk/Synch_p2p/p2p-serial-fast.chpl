/*
   Chapel's serial synch_p2p implementation

   Contributed by Ben Albrecht (Cray Inc.)
*/
use Time;

param PRKVERSION = "2.15";

config const iterations = 10,
             m = 1000,
             n = 100,
             debug: bool = false,
             correctness: bool = false;

//
// Process and test input configs
//
if (iterations < 1) {
  writeln("ERROR: iterations must be >= 1 : ", iterations);
  exit(1);
}
if (m < 1 || n < 1) {
  writeln("ERROR: grid dimensions must be positive:", m, ", ", n);
  exit(1);
}

var timer: stopwatch;

// Initialize and zero out vector
const mrange = 0 .. # m,
      nrange = 0 .. # n,
      outerDom = {mrange, nrange},
      innerDom = outerDom(1.., 1..);

var vector : [outerDom] real;

//
// Print information before main loop
//
if (!correctness) {
  writeln("Parallel Research Kernels version ", PRKVERSION);
  writeln("Serial pipeline execution on 2D grid");
  writeln("Grid sizes                = ", m, ", ", n);
  writeln("Number of iterations      = ", iterations);
}

// Set boundary values (top and left side of grid)
for j in nrange do vector[0,j] = j;
for i in mrange do vector[i,0] = i;

//
// Main loop
//
for iteration in 0 .. iterations {

  // Start timer after warmup iteration
  if (iteration == 1) then timer.start();

  for (i,j) in innerDom {
    vector[i, j] = vector[i-1, j] + vector[i, j-1] - vector[i-1, j-1];
  }

  // Copy bottom right corner value to top left, creating dependency
  vector[0, 0] = -vector[m-1, n-1];

} // end of iterations

timer.stop();

//
// Analyze and output results
//

// Timing
var pipelineTime = timer.elapsed(),
    avgTime = pipelineTime / iterations;

// Error threshold
const epsilon = 1.e-8;

// Verify correctness, using bottom right value of array
var cornerValue : real = (iterations + 1)*(n + m - 2);
if (abs(vector[m-1, n-1] - cornerValue) / cornerValue > epsilon) {
  writeln("ERROR: checksum ", vector[m-1, n-1], " does not match verification \
      value", cornerValue);
  exit(1);
} else {
  writeln("Solution validates");

  if (debug) then writeln("Verification value = ", cornerValue);

  if (!correctness) {
    writeln("Rate (MFlops/s): ", 1.0e-6 * 2 * ((m-1)*(n-1)) / avgTime,
            " Avg time (s): ", avgTime);
  }
}
