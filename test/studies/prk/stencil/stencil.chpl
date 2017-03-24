//
// Chapel's parallel stencil implementation
//

/* Standard Library */
use Time;
use BlockDist;
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
      stencilSize = if compact then (2*R + 1)**2 else 4*R + 1,
      /* Scalar coefficients of weight matrix elements*/
      coefx : dtype = 1.0,
      coefy : dtype = 1.0,
      /* Determine if tiling is enabled */
      tiling = (tileSize > 0 && tileSize < order);

var timer: Timer;

/* Parallel Research Kernel - Stencil */
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

  /* If tiling is disabled, ensure tiledDom can still be created */
  if (!tiling) then tileSize = 1;

  /* Domain over entire Input/Output matrices */
  const localDom = {0.. # order, 0.. # order},
  /* Domain over entire 'active' region, where Output will be updated */
   innerLocalDom = localDom.expand(-R),
  /* Strided domain over 'active' region - possibly a cleaner way to write...*/
  tiledLocalDom = {R.. # order-2*R by tileSize, R.. # order-2*R by tileSize};

  /* Flavors of parallelism, by distribution */
  const blockDist = new dmap(new Block(localDom)),
      stencilDist = new dmap(new Stencil(innerLocalDom, fluff=(R,R))),
           noDist = defaultDist;

  /* Set distribution based on configs */
  const Dist =  if useBlockDist then blockDist
                else if useStencilDist then stencilDist
                else noDist;

  const outputDist =  if useBlockDist then blockDist
                      else if useStencilDist then blockDist
                      else noDist;

  /* Map domains to selected distribution */
  const Dom = localDom dmapped Dist,
   innerDom = innerLocalDom dmapped Dist,
   tiledDom = tiledLocalDom dmapped Dist;

  const outputDom = localDom dmapped outputDist;

  /* Input and Output matrices represented as arrays over a 2D domain */
  var input: [Dom] dtype = 0.0,
      output: [outputDom] dtype = 0.0;

  /* Weight matrix represented as tuple of tuples*/
  var weight: Wsize*(Wsize*(dtype));

  for i in 1..R {
    const element : dtype = 1 / (2*i*R) : dtype;
    weight[R1][R1+i]  =  element;
    weight[R1+i][R1]  =  element;
    weight[R1-i][R1] = -element;
    weight[R1][R1-i] = -element;
  }

  /* Initialize Input matrix */
  [(i, j) in Dom] input[i,j] = coefx*i+coefy*j;

  /* Update ghost cells with initial values */
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
  // Main loop of Stencil
  //
  if debug then startVdebug("stencil-fast-vis");
  for iteration in 0..iterations {

    /* Start timer after warmup iteration */
    if (iteration == 1) {
      timer.start();
    }

    if debug then diagnostics('stencil');
    if (!tiling) {
      forall (i,j) in innerDom with (in weight) {
        var tmpout: dtype = 0.0;
        if (!compact) {
          for param jj in -R..-1 do tmpout += weight[R1][R1+jj] * input[i, j+jj];
          for param jj in 1..R   do tmpout += weight[R1][R1+jj] * input[i, j+jj];
          for param ii in -R..-1 do tmpout += weight[R1+ii][R1] * input[i+ii, j];
          for param ii in 1..R   do tmpout += weight[R1+ii][R1] * input[i+ii, j];
        } else {
          for ii in -R..R do
            for jj in -R..R do
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
              for ii in -R..R do
                for jj in -R..R do
                  tmpout += weight[R1+ii][R1+jj] * input[i+ii, j+jj];
            }
            output[i, j] += tmpout;
          }
        }
      }
    }

    /* Add constant to solution to force refresh of neighbor data */
    if debug then diagnostics('input += 1');
    forall (i,j) in Dom {
      input[i, j] += 1.0;
    }

    /* Update ghost cells for each locales, for StencilDist */
    if useStencilDist then {
      if debug then diagnostics('input.updateFluff()');
      input.updateFluff();
    }


  } /* end of main loop */

  timer.stop();
  if debug then stopVdebug();

  //
  // Analyze and output results
  //

  /* Timings */
  var stencilTime = timer.elapsed(),
      flops = (2*stencilSize + 1) * activePoints,
      avgTime = stencilTime / iterations;

  /* Compute L1 norm */
  var referenceNorm = (iterations + 1) * (coefx + coefy),
      norm = + reduce abs(output);
  norm /= activePoints;

  /* Error threshold */
  const epsilon = 1.e-8;

  /* Verify correctness */
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


/* Helper function for merging debug output and chplvis calls

  :arg tag: Name of chplvis tag produced, and outputted during execution

 */
proc diagnostics(tag: string) {
  tagVdebug(tag);
  writeln('[Debug]: ', tag);
}
