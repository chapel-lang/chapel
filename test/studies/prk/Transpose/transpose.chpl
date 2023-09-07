/*
   Chapel's parallel Transpose implementation

   Contributed by Ben Albrecht (Cray Inc.),
                  Engin Kayraklioglu (GWU)
*/

use Time;
use BlockDist;

param PRKVERSION = "2.17";

config param useBlockDist = false;

config const iterations = 100,
             order = 100,
             correctness = false,
             debug = false;

config var tileSize = 0;

//
// Process and test input configs
//
if (iterations < 1) {
  halt("ERROR: iterations must be >= 1: ", iterations);
}

if (order < 0) {
  halt("ERROR: Matrix Order must be greater than 0 : ", order);
}

if (tileSize > order) {
  halt("ERROR: Tile size cannot be larger than order");
}

// Determine tiling
const tiled = tileSize > 0;

// Safety check for creation of tiledDom
if (!tiled) then tileSize = 1;

// Domains
const localDom = {0..#order, 0..#order};
var tiledLocalDom = {0..#order by tileSize, 0..#order by tileSize};


const myBlockDist = new blockDist(localDom);
const Dist =  if useBlockDist then myBlockDist
                              else defaultDist;

const Dom = localDom dmapped Dist;
const tiledDom = tiledLocalDom dmapped Dist;

var timer: stopwatch,
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
[(i, j) in Dom with (ref A)] A[i,j] = order*j + i;

// Initialize B for clarity
B = 0.0;

//
// Main loop
//
for iteration in 0..iterations {
  // Start timer after a warmup lap
  if (iteration == 1) then timer.start();

  if (tiled) {
    forall (i,j) in tiledDom with (ref A, ref B) {
      for it in i .. # min(order - i, tileSize) {
        for jt in j .. # min(order - j, tileSize) {
          B[jt,it] += A[it,jt];
          A[it,jt] += 1.0;
        }
      }
    }
  }
  else {
    forall (i,j) in Dom with (ref A, ref B) {
      B[j,i] += A[i,j];
      A[i,j] += 1.0;
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
const addit = ((iterations+1) * iterations)/2.0;
var absErr = + reduce [(i,j) in Dom]
    abs(B[i,j]-((order*i+j)*(iterations+1)+addit));

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
