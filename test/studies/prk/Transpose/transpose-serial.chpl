/*
   Chapel's serial Transpose implementation

   Contributed by Ben Albrecht (Cray Inc.)
*/

use Time;

param PRKVERSION = "2.15";

config const iterations : int = 100,
             order : int = 100,
             debug: bool = false,
             correctness: bool = false;

config var tileSize: int = 0;

//
// Process and test input configs
//
if (iterations < 1) {
  writeln("ERROR: iterations must be >= 1: ", iterations);
  exit(1);
}
if (order < 0) {
  writeln("ERROR: Matrix Order must be greater than 0 : ", order);
  exit(1);
}

// Determine tiling
var tiled = (tileSize < order && tileSize > 0);

// Safety check for creation of tiledDom
if (!tiled) then tileSize = 1;

// Domains
const    Dom = {0.. # order, 0.. # order};
var tiledDom = {0.. # order by tileSize, 0.. # order by tileSize};

var timer: Timer,
    nBytes = 2.0 * numBytes(real) * order * order,
    A, B : [Dom] real;

//
// Print information before main loop
//
if (!correctness) {
  writeln("Parallel Research Kernels version ", PRKVERSION);
  writeln("Serial Matrix transpose: B = A^T");
  writeln("Matrix order          = ", order);
  if (tiled) then writeln("Tile size              = ", tileSize);
  else            writeln("Untiled");
  writeln("Number of iterations = ", iterations);
}

// Fill original column matrix
[(i, j) in Dom] A[i,j] = order*j + i;

// Set transpose matrix to known garbage value
B = -1.0;

//
// Main loop
//
for iteration in 0..iterations {
  // Start timer after a warmup lap
  if (iteration == 1) then timer.start();

  if (tiled) {
    for (i,j) in tiledDom {
      for it in i .. # min(order - i, tileSize) {
        for jt in j .. # min(order - j, tileSize) {
          B[jt,it] = A[it,jt];
        }
      }
    }
  }
  else {
    for (i,j) in Dom {
      B[j,i] = A[i,j];
    }
  }

} // end of main loop

timer.stop();

//
// Analyze and output results
//

// Timings
var transposeTime = timer.elapsed(),
    avgTime = transposeTime / iterations;

// Error tolerance
const epsilon = 1.e-8;

var absErr = 0.0;
for (i,j) in Dom {
  absErr += abs(B[i,j] - (order*i + j));
}

if (debug) {
  writeln("transposeTime = ", transposeTime);
  writeln("Sum of absolute differences: ", absErr);
}

// Verify correctness
if (absErr < epsilon) {
  writeln("Solution validates");
  if (!correctness) then writeln("Rate (MB/s): ", 1.0E-06 * nBytes / avgTime,
                              " Avg time (s): ", avgTime);
} else {
  writeln("ERROR: Aggregate squared error", absErr,
          " exceeds threshold ", epsilon);
}
