// Chapel's serial stencil implementation
use PRK;

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
      stencilSize = 4*R + 1,
      weightSize = 2*R + 1,
      coefx : dtype = 1.0,
      coefy : dtype = 1.0,
      epsilon = 1.e-8;

// Timer
var timer: Timer;

// Domains
const Dom = {0.. # n, 0.. # n},
      InnerDom = Dom.expand(-R),
      W = {-R..R, -R..R};

// Arrays
var input, output: [Dom] dtype = 0.0;
var weight: [W] dtype = 0.0;

for i in 1..R do {
  const element : dtype = 1 / (2*i*R) : dtype;
  weight[0, i]  =  element;
  weight[i, 0]  =  element;
  weight[-i, 0] = -element;
  weight[0, -i] = -element;
}
writeln(weight);

// Initialize the input and output arrays
serial do [(i, j) in Dom] input[i,j] = coefx*i + coefy*j;

// Print information before main loop
writeln("Parallel Research Kernels Version ", PRKVERSION);
writeln("Serial stencil execution on 2D grid");
writeln("Grid size            = ", n);
writeln("Radius of stencil    = ", R);
writeln("Type of stencil      = star"); // Temporarily hard-coded
writeln("Data type            = double precision");
writeln("Untiled");                     // Temporarily hard-coded
writeln("Number of iterations = ", iterations);

for iteration in 0..iterations do {
  // Start timer after warmup iteration
  if (iteration == 1) {
    timer.start();
  }

  for (i,j) in InnerDom {
    for param jj in -R..R  do output[i, j] += weight[0, jj] * input[i, j+jj];
    for param ii in -R..-1 do output[i, j] += weight[ii, 0] * input[i+ii, j];
    for param ii in 1..R   do output[i, j] += weight[ii, 0] * input[i+ii, j];
  }


  // Add constant to solution to force refresh of neighbor data, if any
  for (i,j) in Dom do input[i,j] += 1.0;

} // end of iterations

timer.stop();

// Timings
var stencilTime = timer.elapsed();
writeln("stencil_time: ", stencilTime);

// Compute L1 norm in parallel
var norm = + reduce abs(output);

norm /= activePoints;

/*******************************************************************************
** Analyze and output results.
********************************************************************************/

// Verify correctness
var referenceNorm = (iterations + 1) * (coefx + coefy);

if abs(norm-referenceNorm) > epsilon then {
  writeln("ERROR: L1 norm = ", norm, ", Reference L1 norm = ", referenceNorm);
  exit(1);
} else {
  writeln("Solution validates");
  if debug then {
    writeln("L1 norm = ", norm, ", Reference L1 norm = ", referenceNorm);
  }
}

var flops = (2*stencilSize + 1) * activePoints;
var avgTime = stencilTime / iterations;
writeln("Rate (MFlops/s): ", 1.0E-06 * flops/avgTime,
        "  Avg time (s): ", avgTime);
