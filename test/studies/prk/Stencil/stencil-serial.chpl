/*
   Chapel's serial Stencil implementation

   Contributed by Ben Albrecht, Ben Harshbarger (Cray Inc.)
*/

use Time;

param PRKVERSION = "2.15";

config var tileSize: int = 0;

config const iterations: int = 10,
             order: int = 1000,
             debug: bool = false,
             correctness: bool = false;

config param R = 2,
             compact = false;

// Configurable type for array elements
config type dtype = real;

const activePoints = (order-2*R)*(order-2*R),
      coefx: dtype = 1.0,
      coefy: dtype = 1.0;

param stencilSize = 4*R + 1,
      weightSize = 2*R + 1,
      Wsize = 2*R + 1;

var timer: Timer;

//
// Process and test input configs
//
if (iterations < 1) {
  writeln("ERROR: iterations must be >= 1: ", iterations);
  exit(1);
}
if (order < 1) {
  writeln("ERROR: Matrix Order must be greater than 0 : ", order);
  exit(1);
}
if (R < 1) {
  writeln("ERROR: Stencil radius ", R, " should be positive");
  exit(1);
}
if (2*R + 1 > order) {
  writeln("ERROR: Stencil radius ", R, " exceeds grid size ", order);
  exit(1);
}

// Determine tiling
var tiling = (tileSize > 0 && tileSize < order);

// Safety check for creation of tiledDom
if (!tiling) then tileSize = 1;

// Domains
const    Dom = {0.. # order, 0.. # order},
    innerDom = Dom.expand(-R),
   weightDom = {-R..R, -R..R};

var tiledDom = {R.. # order-2*R by tileSize, R.. # order-2*R by tileSize};

// Arrays
var input, output: [Dom] dtype = 0.0;
var tmpout : dtype = 0.0;

// Tuple of tuples
var weight: Wsize*(Wsize*(dtype));

// Set up weight matrix
for i in 1..R {
  const element : dtype = 1 / (2*i*R) : dtype;
  weight[R][R+i]  =  element;
  weight[R+i][R]  =  element;
  weight[R-i][R] = -element;
  weight[R][R-i] = -element;
}

// Initialize the input and output arrays
[(i, j) in Dom] input[i,j] = coefx*i + coefy*j;

//
// Print information before main loop
//
if (!correctness) {
  writeln("Parallel Research Kernels Version ", PRKVERSION);
  writeln("Serial stencil execution on 2D grid");
  writeln("Grid size            = ", order);
  writeln("Radius of stencil    = ", R);
  if compact then writeln("Type of stencil      = compact");
  else              writeln("Type of stencil      = star");
  writeln("Data type            = ", dtype:string);
  if tiling then writeln("Tile size             = ", tileSize);
  else             writeln("Untiled");
  writeln("Number of iterations = ", iterations);
}

//
// Main loop
//
for iteration in 0..iterations {

  // Start timer after warmup iteration
  if (iteration == 1) {
    timer.start();
  }

  if (!tiling) {
    for (i,j) in innerDom {
      var tmpout: dtype = 0.0;
      if (!compact) {
        for param jj in -R..-1 do tmpout += weight[R][R+jj] * input[i, j+jj];
        for param jj in 1..R   do tmpout += weight[R][R+jj] * input[i, j+jj];
        for param ii in -R..-1 do tmpout += weight[R+ii][R] * input[i+ii, j];
        for param ii in 1..R   do tmpout += weight[R+ii][R] * input[i+ii, j];
      } else {
        for (ii, jj) in weightDom do
          tmpout += weight[R+ii][R+jj] * input[i+ii, j+jj];
      }
      output[i, j] += tmpout;
    }
  } else {
    for (it,jt) in tiledDom {
      for i in it .. # min(order - R - it, tileSize) {
        for j in jt .. # min(order - R - jt, tileSize) {
          var tmpout: dtype = 0.0;
          if (!compact) {
            for param jj in -R..-1 do tmpout += weight[R][R+jj] * input[i, j+jj];
            for param jj in 1..R   do tmpout += weight[R][R+jj] * input[i, j+jj];
            for param ii in -R..-1 do tmpout += weight[R+ii][R] * input[i+ii, j];
            for param ii in 1..R   do tmpout += weight[R+ii][R] * input[i+ii, j];
          } else {
            for (ii, jj) in weightDom do
              tmpout += weight[R+ii][R+jj] * input[i+ii, j+jj];
          }
          output[i, j] += tmpout;
        }
      }
    }
  }

  // Add constant to solution to force refresh of neighbor data, if any
  for (i,j) in Dom do input[i,j] += 1.0;

} // end of iterations

timer.stop();

//
// Analyze and output results
//

// Timings
var stencilTime = timer.elapsed(),
    flops = (2*stencilSize + 1) * activePoints,
    avgTime = stencilTime / iterations;

// Compute L1 norm
var referenceNorm = (iterations + 1) * (coefx + coefy),
    norm = + reduce abs(output);
norm /= activePoints;

// Error threshold
const epsilon = 1.e-8;

// Verify correctness
if abs(norm-referenceNorm) > epsilon then {
  writeln("ERROR: L1 norm = ", norm, ", Reference L1 norm = ", referenceNorm);
  exit(1);
} else {
  writeln("Solution validates");

  if debug {
    writeln("L1 norm = ", norm, ", Reference L1 norm = ", referenceNorm);
  }

  if (!correctness) {
    writef("Rate (MFlops/s): %dr  Avg time (s): %r\n", 1.0E-06 * flops/avgTime, avgTime);
  }
}
