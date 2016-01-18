// Chapel's serial stencil implementation
use Time;

param PRKVERSION = "2.15";

config const iterations: int = 10,
             order: int = 1000,
             tileSize: int = 0;

// Additional output for debugging and reduced output for validation
config const debug: bool = false,
             validate: bool = false;

// Stencil radius
config param R = 2,
             compact = false; // not yet implemented

// Configurable type for array elements
config type dtype = real;

// Runtime constants
const activePoints = (order-2*R)*(order-2*R),
      stencilSize = 4*R + 1,
      weightSize = 2*R + 1,
      coefx : dtype = 1.0,
      coefy : dtype = 1.0;

// Timer
var timer: Timer;

// Domains
const Dom = {0.. # order, 0.. # order},
      InnerDom = Dom.expand(-R),
      W = {-R..R, -R..R};

// Arrays
var input, output: [Dom] dtype = 0.0;
var weight: [W] dtype = 0.0;

// Process and test input configs
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

for i in 1..R do {
  const element : dtype = 1 / (2*i*R) : dtype;
  weight[0, i]  =  element;
  weight[i, 0]  =  element;
  weight[-i, 0] = -element;
  weight[0, -i] = -element;
}

// Initialize the input and output arrays
[(i, j) in Dom] input[i,j] = coefx*i + coefy*j;

// Print information before main loop
if (!validate) {
  writeln("Parallel Research Kernels Version ", PRKVERSION);
  writeln("Serial stencil execution on 2D grid");
  writeln("Grid size            = ", order);
  writeln("Radius of stencil    = ", R);
  if compact then writeln("Type of stencil      = compact"); // TODO
  else              writeln("Type of stencil      = star");
  writeln("Data type            = ", dtype:string);
  if tiling then writeln("Tile size             = ", tileSize); // TODO
  else             writeln("Untiled");
  writeln("Number of iterations = ", iterations);
}


for iteration in 0..iterations do {

  // Start timer after warmup iteration
  if (iteration == 1) {
    timer.start();
  }

  for (i,j) in InnerDom {
    for jj in -R..R  do output[i, j] += weight[0, jj] * input[i, j+jj];
    for ii in -R..-1 do output[i, j] += weight[ii, 0] * input[i+ii, j];
    for ii in 1..R   do output[i, j] += weight[ii, 0] * input[i+ii, j];
  }

  // Add constant to solution to force refresh of neighbor data, if any
  input += 1.0;

} // end of iterations

timer.stop();

// Analyze and output results

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

  if (!validate) {
    writeln("Rate (MFlops/s): ", 1.0E-06 * flops/avgTime, "  Avg time (s): ", 
            avgTime);
  }
}
