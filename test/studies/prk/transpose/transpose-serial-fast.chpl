// Chapel's serial implementation of transpose

use Time;

param PRKVERSION = "2.15";

config const iterations : int = 100;
config const order : int = 100;
config const tileSize : int = 32;

// Additional output for debugging
config const debug: bool = false;

// Only print the validation string
config const validate: bool = false;

// Timer
var timer: Timer;

var bytes = 2.0 * numBytes(real) * order * order;
var tiled = false;

const Dom = {0.. # order, 0.. # order},
 tiledDom = {0.. # order by tileSize, 0.. # order by tileSize};
var A, B : [Dom] real;


if (!validate) {
  writeln("Parallel Research Kernels version ", PRKVERSION);
  writeln("Serial Matrix transpose: B = A^T");
  writeln("Matrix order          = ", order);
  if (tileSize < order && tileSize > 0) {
    writeln("Tile size              = ", tileSize);
    tiled = true;
  } else {
    writeln("Untiled");
    tiled = false;
  }
  writeln("Number of iterations = ", iterations);
}

// Fill original column matrix
[(i, j) in Dom] A[i,j] = order*j + i;

// Set transpose matrix to known garbage value
B = -1.0;

// Main loop
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

// Analyze and output results

var transposeTime = timer.elapsed();
var avgTime = transposeTime / iterations;

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

if (absErr < epsilon) {
  writeln("Solution validates");
  if (!validate) then writeln("Rate (MB/s): ", 1.0E-06 * bytes / avgTime,
                              " Avg time (s): ", avgTime);
} else {
  writeln("ERROR: Aggregate squared error", absErr,
          " exceeds threshold ", epsilon);
}
