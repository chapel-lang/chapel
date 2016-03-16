//
// Chapel's parallel stencil implementation
//

/* Standard Library */
use Time;
use BlockDist;
use ReplicatedDist;
use VisualDebug;

/* Included from miniMD benchmark */
use StencilDist;

/* Version kept in sync with PRK repository */
param PRKVERSION = "2.16";

/* Numerical type for matrix elements */
config type dtype = real;

/* Radius of weight matrix */
config param R = 2,
             /* Determine weight matrix shapre; square = false, star = true */
             compact = false,
             /* Map domains to Stencil Distribution */
             useStencilDist = false,
             /* Map domains to Block Distribution */
             useBlockDist = false;
             /* No domain mapping, if neither is selected (shared) */

/* Number of iterations to execute (0th iteration is untimed) */
config const iterations: int = 10,
             /* Input/Output matrix dimensions are 'order' x 'order' */
             order: int = 1000,
             /* Enable debug output, including chplvis data */
             debug: bool = false,
             /* Only print result of validation - used in correctness tests*/
             validate: bool = false;

/* Size of stride for tiling; disables tiling if set to 0 */
config var tileSize: int = 0;

/* Weight matrix dimensions are 'Wsize' x 'Wsize' */
param Wsize = 2*R + 1,
      /* Frequently used constant for tuple index bookkeeping*/
      R1 = R + 1;


/* Number of elements that will be updated in Output matrix */
const activePoints = (order-2*R)*(order-2*R),
      /* Number of elements in weight matrix, for calculating flops */
      stencilSize = if compact then (2*R + 1)**2 else 4*R + 1;
      /* Scalar coefficients of weight matrix elements*/
      coefx : dtype = 1.0,
      coefy : dtype = 1.0;

var timer: Timer;

proc main() {
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
  const blockDist = new Block(localDom),
      stencilDist = new Stencil(innerLocalDom, fluff=(R,R)),
           noDist = new DefaultDist(),
         replDist = new ReplicatedDist();

  const Dist =  if useBlockDist then blockDist
                else if useStencilDist then stencilDist
                else noDist;


  const Dom = localDom dmapped new dmap(Dist),
   innerDom = innerLocalDom dmapped new dmap(Dist);

  var tiledDom = {R.. # order-2*R by tileSize, R.. # order-2*R by tileSize};

  // Input and Output arrays
  var input, output:  [Dom] dtype = 0.0;


  // Tuple of tuples
  pragma "locale private"
  var weight: Wsize*(Wsize*(dtype));

  // Create local copy of weight on each Locale
  for loc in Locales do on loc {
    for i in 1..R {
      const element : dtype = 1 / (2*i*R) : dtype;
      weight[R1][R1+i]  =  element;
      weight[R1+i][R1]  =  element;
      weight[R1-i][R1] = -element;
      weight[R1][R1-i] = -element;
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
    else            writeln("Type of stencil      = star");
    writeln("Data type            = ", dtype:string);
    if tiling then writeln("Tile size             = ", tileSize);
    else           writeln("Untiled");
    writeln("Number of iterations = ", iterations);
    if useBlockDist then        writeln("Distribution         = Block");
    else if useStencilDist then writeln("Distribution         = Stencil");
    else                        writeln("Distribution         = None");
  }


  //
  // Main loop
  //
  if debug then startVdebug("stencil-fast-vis");
  for iteration in 0..iterations {

    // Start timer after warmup iteration
    if (iteration == 1) {
      timer.start();
    }

    if debug then tagVdebug('stencil');
    if (!tiling) {
      forall (i,j) in innerDom {
        var tmpout: dtype = 0.0;
        if (!compact) {
          for param jj in -R..-1 do tmpout += weight[R1][R1+jj] * input[i, j+jj];
          for param jj in 1..R   do tmpout += weight[R1][R1+jj] * input[i, j+jj];
          for param ii in -R..-1 do tmpout += weight[R1+ii][R1] * input[i+ii, j];
          for param ii in 1..R   do tmpout += weight[R1+ii][R1] * input[i+ii, j];
        } else {
          for (ii, jj) in (-R..R, -R..R) do
            tmpout += weight[R1+ii][R1+jj] * input[i+ii, j+jj];
        }
        output[i, j] += tmpout;
      }
    } else {
      forall (it,jt) in tiledDom {
        for i in it .. # min(order - R - it, tileSize) {
          for j in jt .. # min(order - R - jt, tileSize) {
            var tmpout: dtype = 0.0;
            if (!compact) {
              for param jj in -R..-1 do tmpout += weight[R1][R1+jj] * input[i, j+jj];
              for param jj in 1..R   do tmpout += weight[R1][R1+jj] * input[i, j+jj];
              for param ii in -R..-1 do tmpout += weight[R1+ii][R1] * input[i+ii, j];
              for param ii in 1..R   do tmpout += weight[R1+ii][R1] * input[i+ii, j];
            } else {
              for (ii, jj) in (-R..R, -R..R) do
                tmpout += weight[R1+ii][R1+jj] * input[i+ii, j+jj];
            }
            output[i, j] += tmpout;
          }
        }
      }
    }

    // Add constant to solution to force refresh of neighbor data, if any
    if debug then diagnostics('input += 1');
    forall (i,j) in Dom {
      input[i, j] += 1.0;
    }

    if useStencilDist then {
      if debug then diagnostics('output.updateFluff()');
      output.updateFluff();
    }


  } // end of iterations

  timer.stop();
  if debug then stopVdebug();

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
}

proc diagnostics(tag: string) {
  tagVdebug(tag);
  writeln('[Debug]: ', tag);
}
