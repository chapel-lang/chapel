//
// Chapel's shared parallel stencil implementation
//
use Time;
use BlockDist;
use ReplicatedDist;
use StencilDist; // Included from miniMD

param PRKVERSION = "2.15";

config var tileSize: int = 0;

config const iterations: int = 10,
             order: int = 1000,
             // Output controls
             debug: bool = false,
             validate: bool = false;

config param R = 2,
             // Square weight matrix (true) or Star weight matrix (false)
             compact = false,
             // Control of multilocale parallelism
             useStencilDist = false,
             useBlockDist = (CHPL_COMM != "none" && !useStencilDist);

// Configurable type for array elements
config type dtype = real;

// Runtime constants
const activePoints = (order-2*R)*(order-2*R),
      stencilSize = 4*R + 1,
      weightSize = 2*R + 1,
      coefx : dtype = 1.0,
      coefy : dtype = 1.0;

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

// Domain Map

const localDom = {0.. # order, 0.. # order},
 innerLocalDom = localDom.expand(-R),
weightLocalDom = {-R..R, -R..R};

// Choice of distribution / parallelism
const blockDist = new dmap(new Block(localDom)),
    stencilDist = new dmap(new Stencil(innerLocalDom, fluff=(R,R))),
         noDist = new dmap(new DefaultDist()),
       replDist = new dmap(new ReplicatedDist());

const Dist =  if useBlockDist then blockDist
              else if useStencilDist then stencilDist
              else noDist;

const weightDist = if (useBlockDist || useStencilDist) then replDist
                 else noDist;

// Domains
const Dom = localDom dmapped Dist,
 innerDom = innerLocalDom dmapped Dist,
weightDom = weightLocalDom dmapped weightDist;

var tiledDom = {R.. # order-2*R by tileSize, R.. # order-2*R by tileSize};

// Arrays (initialized to zeros)
var input, output:  [Dom] dtype = 0.0,
    weight: [weightDom] dtype = 0.0;

// Create local copy of weight on each Locale
for L in Locales do on L {
  forall i in 1..R do {
    const element = 1.0 / (2.0*i*R);
    weight[0, i]  =  element;
    weight[i, 0]  =  element;
    weight[-i, 0] = -element;
    weight[0, -i] = -element;
  }
}

// Initialize the input and output arrays
[(i, j) in Dom] input[i,j] = coefx*i+coefy*j;

if useStencilDist then input.updateFluff();

//
// Print information before main loop
//
if (!validate) {
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
    forall (i,j) in innerDom {
      if (!compact) {
        for param jj in -R..R  do output[i, j] += weight[0, jj] * input[i, j+jj];
        for param ii in -R..-1 do output[i, j] += weight[ii, 0] * input[i+ii, j];
        for param ii in 1..R   do output[i, j] += weight[ii, 0] * input[i+ii, j];
      } else {
        for (ii, jj) in weightDom do
          output[i, j] += weight[ii,jj] * input[i+ii, j+jj];
      }
    }
  } else {
    forall (it,jt) in tiledDom {
      for i in it .. # min(order - R - it, tileSize) {
        for j in jt .. # min(order - R - jt, tileSize) {
          if (!compact) {
            for param jj in -R..R  do output[i, j] += weight[0, jj] * input[i, j+jj];
            for param ii in -R..-1 do output[i, j] += weight[ii, 0] * input[i+ii, j];
            for param ii in 1..R   do output[i, j] += weight[ii, 0] * input[i+ii, j];
          } else {
            for (ii, jj) in weightDom do
              output[i, j] += weight[ii,jj] * input[i+ii, j+jj];
          }
        }
      }
    }
  }

  if useStencilDist then input.updateFluff();

  // Add constant to solution to force refresh of neighbor data, if any
  forall (i,j) in Dom {
    input[i, j] += 1.0;
  }

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

  if (!validate) {
    writeln("Rate (MFlops/s): ", 1.0E-06 * flops/avgTime, "  Avg time (s): ", 
            avgTime);
  }
}
