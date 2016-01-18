// Chapel's shared-memory parallel stencil implementation
use Time;

param PRKVERSION = "2.15";

// Note: Defaulting to STAR stencil (defines weight)
// Configurable runtime constants
config const iterations: int = 10,
             n: int = 1000,
             debug: bool = false;

config type dtype = real;

// Configurable compile constants
config param R = 2;

// Runtime constants
const activePoints = (n-2*R)*(n-2*R),
      coefx : dtype = 1.0,
      coefy : dtype = 1.0;

param stencilSize = 4*R + 1,
      weightSize = 2*R + 1,
      epsilon = 1.e-8;

// Timer
var timer: Timer;

// Domains
const    Dom = {0.. # n, 0.. # n},
    InnerDom = Dom.expand(-R),
           W = {-R..R, -R..R};

// Arrays (initialized to zeros)
var input, output:  [Dom] dtype = 0.0,
    weight: [W] dtype = 0.0;

forall i in 1..R do {
  const element = 1.0 / (2.0*i*R);
  weight[0, i]  =  element;
  weight[i, 0]  =  element;
  weight[-i, 0] = -element;
  weight[0, -i] = -element;
}

// Initialize the input and output arrays
[(i, j) in Dom] input[i,j] = coefx*i+coefy*j;

// Print information before main loop
writeln("Parallel Research Kernels Version ", PRKVERSION);
writeln("Serial stencil execution on 2D grid");
writeln("Grid size            = ", n);
writeln("Radius of stencil    = ", R);
writeln("Type of stencil      = star"); // Temporarily hard-coded
writeln("Data type            = double precision");
writeln("Untiled");                     // Temporarily hard-coded
writeln("Number of iterations = ", iterations);

for iteration in 0..iterations {

  // Start timer after warmup iteration
  if (iteration == 1) {
    timer.start();
  }

  forall (i,j) in InnerDom {
    for param jj in -R..R  do output[i, j] += weight[0, jj] * input[i, j+jj];
    for param ii in -R..-1 do output[i, j] += weight[ii, 0] * input[i+ii, j];
    for param ii in 1..R   do output[i, j] += weight[ii, 0] * input[i+ii, j];
  }

  // Add constant to solution to force refresh of neighbor data, if any
  forall (i,j) in Dom {
    input[i, j] += 1.0;
  }

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
