/*
  A distributed 2D finite-difference heat/diffusion equation solver

  Computation is executed over a 2D distributed array.
  The array distribution is managed by the `Block` distribution.
  Tasks are spawned manually with a `coforall` loop and synchronization
  is done manually using a `barrier`. Halo regions are shared across
  locales manually via halo/buffer arrays.
*/

import BlockDist.Block,
       Collectives.barrier,
       Time.stopwatch;

// create a stopwatch to time kernel execution
var t = new stopwatch();
config const writeTime = false;

// compile with `-sRunCommDiag=true` to see comm diagnostics
use CommDiagnostics;
config param RunCommDiag = false;

// declare configurable constants with default values
config const nx = 256,      // number of grid points in x
             ny = 256,      // number of grid points in y
             nt = 50,       // number of time steps
             alpha = 0.25,  // diffusion constant
             solutionStd = 0.222751; // know solution for the default parameters

// define distributed domains and block-distributed array
const indices = {0..<nx, 0..<ny},
      indicesInner = indices.expand(-1),
      Indices = Block.createDomain(indices);

// define a distributed 2D array over the above domain
var u: [Indices] real;

// apply initial conditions
u = 1.0;
u[nx/4..nx/2, ny/4..ny/2] = 2.0;

// array wrapper for creating a "skyline" array of halo buffers
record haloArray {
  var d: domain(1);
  var v: [d] real;

  proc init() do this.d = {1..0};
  proc init(r: range(int)) do
    this.d = {r};
}

// set up array of halo buffers over same distribution as 'u.targetLocales'
var OnePerLocale = Block.createDomain(u.targetLocales().domain);
var HaloArrays: [OnePerLocale] [0..<4] haloArray;

// buffer edge indices: North, East, South, West
param N = 0, S = 1, E = 2, W = 3;

// number of tasks that will be created per dimension based on the
//  Block distribution's 2D decomposition (with one task per locale)
const tidXMax = OnePerLocale.dim(0).high,
      tidYMax = OnePerLocale.dim(1).high;

// barrier for one task per locale
var b = new barrier(OnePerLocale.size);

proc main() {
  if RunCommDiag then startCommDiagnostics();

  // solve, spawning one task for each locale
  t.start();
  forall (tidX, tidY) in OnePerLocale {
    const localDom = u.localSubdomain();

    // allocate halo arrays
    HaloArrays[tidX, tidY][N] = new haloArray(localDom.dim(1).expand(1));
    HaloArrays[tidX, tidY][S] = new haloArray(localDom.dim(1).expand(1));
    HaloArrays[tidX, tidY][E] = new haloArray(localDom.dim(0).expand(1));
    HaloArrays[tidX, tidY][W] = new haloArray(localDom.dim(0).expand(1));

    // synchronize across tasks
    b.barrier();

    // run the portion of the FD computation owned by this task
    work(tidX, tidY);
  }
  t.stop();

  if RunCommDiag {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
  }

  // print final results
  const mean = (+ reduce u) / u.size,
        stdDev = sqrt((+ reduce (u - mean)**2) / u.size);

  writeln(abs(solutionStd - stdDev) < 1e-6);
  if writeTime then writeln("time: ", t.elapsed());
}

proc work(tidX: int, tidY: int) {
  // define domains to describe the indices owned by this task
  const localIndices = u.localSubdomain(here),
        localIndicesBuffered = localIndices.expand(1),
        localIndicesInner = localIndices[indicesInner];

	// declare two local arrays for computation on this locale
  var uLocal1, uLocal2: [localIndicesBuffered] real = 1;

  // populate local arrays with initial conditions from global array
  uLocal1[localIndices] = u[localIndices];
  uLocal2 = uLocal1;

  // define constants for indexing into edges of local array
  const WW = localIndicesBuffered.dim(1).low,
        EE = localIndicesBuffered.dim(1).high,
        SS = localIndicesBuffered.dim(0).high,
        NN = localIndicesBuffered.dim(0).low;

  // iterate for 'nt' time steps
  for 1..nt {
    // store results from last iteration in neighboring task's halo buffers
    if tidY > 0       then HaloArrays[tidX, tidY-1][E].v = uLocal2[.., WW+1];
    if tidY < tidYMax then HaloArrays[tidX, tidY+1][W].v = uLocal2[.., EE-1];
    if tidX > 0       then HaloArrays[tidX-1, tidY][S].v = uLocal2[NN+1, ..];
    if tidX < tidXMax then HaloArrays[tidX+1, tidY][N].v = uLocal2[SS-1, ..];

    // synchronize with other tasks
    b.barrier();

    // populate edges of local array from halo buffers
    if tidY > 0       then uLocal2[.., WW] = HaloArrays[tidX, tidY][W].v;
    if tidY < tidYMax then uLocal2[.., EE] = HaloArrays[tidX, tidY][E].v;
    if tidX > 0       then uLocal2[NN, ..] = HaloArrays[tidX, tidY][N].v;
    if tidX < tidXMax then uLocal2[SS, ..] = HaloArrays[tidX, tidY][S].v;

    // swap local arrays
    uLocal1 <=> uLocal2;

    // compute the FD kernel in parallel
    forall (i, j) in localIndicesInner do
      uLocal2[i, j] = uLocal1[i, j] + alpha * (
        uLocal1[i-1, j] + uLocal1[i, j-1] +
        uLocal1[i+1, j] + uLocal1[i, j+1] -
        4 * uLocal1[i, j]
      );

    b.barrier();
  }

  // store results in global array
  uLocal1 <=> uLocal2;
  u[localIndices] = uLocal1[localIndices];
}
