/*
  A distributed 2D finite-difference heat/diffusion equation solver

  Computation is executed over a 2D distributed array.
  The array distribution is managed by the `Block` distribution.
  Tasks are spawned manually with a `coforall` loop and synchronization
  is done manually using a `barrier`. Halo regions are shared across
  locales manually via direct assignment between locally owned arrays.
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
var u: [Indices] real;

// apply initial conditions
u = 1.0;
u[nx/4..nx/2, ny/4..ny/2] = 2.0;

// number of tasks per dimension based on Block distributions decomposition
const tidXMax = u.targetLocales().dim(0).high,
      tidYMax = u.targetLocales().dim(1).high;

// barrier for one task per locale
var b = new barrier(u.targetLocales().size);

// a type for creating a "skyline" array of local arrays
record localArray {
  var d: domain(2);
  var v: [d] real;

  proc init() do this.d = {0..0, 0..0};
  proc init(dGlobal: domain(2)) do this.d = dGlobal;
}

// set up an arrays of local arrays over same distribution as 'u.targetLocales'
var OnePerLocale = Block.createDomain(u.targetLocales().domain);
var uTaskLocal, unTaskLocal: [OnePerLocale] localArray;

proc main() {
  if RunCommDiag then startCommDiagnostics();

  // solve, spawning one task for each locale
  t.start();
  coforall (loc, (tidX, tidY)) in zip(u.targetLocales(), OnePerLocale) {
    // run initialization and computation on the task for this locale
    on loc do work(tidX, tidY);
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
  const myGlobalIndices = u.localSubdomain(here),
        localIndicesBuffered = myGlobalIndices.expand(1),
        localIndicesInner = myGlobalIndices[indicesInner];

  // initialize this tasks local arrays using indices from `Block` dist.
  uTaskLocal[tidX, tidY] = new localArray(localIndicesBuffered);
  unTaskLocal[tidX, tidY] = new localArray(localIndicesBuffered);

  // get a reference to this task's local arrays
  ref uLocal = uTaskLocal[tidX, tidY].v,
      unLocal = unTaskLocal[tidX, tidY].v;

  // copy initial conditions from global array
  uLocal = 1;
  uLocal[myGlobalIndices] = u[myGlobalIndices];
  unLocal = uLocal;

  // define constants for indexing into edges of local array
  const N = myGlobalIndices.dim(1).low,
        S = myGlobalIndices.dim(1).high,
        E = myGlobalIndices.dim(0).high,
        W = myGlobalIndices.dim(0).low;

  b.barrier();

  // define constants for indexing into halo regions of neighboring arrays
  const NN = if tidY < tidYMax then unTaskLocal[tidX, tidY+1].d.dim(1).low else 0,
        SS = if tidY > 0       then unTaskLocal[tidX, tidY-1].d.dim(1).high else 0,
        WW = if tidX < tidXMax then unTaskLocal[tidX+1, tidY].d.dim(0).low else 0,
        EE = if tidX > 0       then unTaskLocal[tidX-1, tidY].d.dim(0).high else 0;

  // run FD computation
  for 1..nt {
    // store results from last iteration in neighboring task's halos
    if tidX > 0       then unTaskLocal[tidX-1, tidY].v[EE, ..] = unLocal[W, ..];
    if tidX < tidXMax then unTaskLocal[tidX+1, tidY].v[WW, ..] = unLocal[E, ..];
    if tidY > 0       then unTaskLocal[tidX, tidY-1].v[.., SS] = unLocal[.., N];
    if tidY < tidYMax then unTaskLocal[tidX, tidY+1].v[.., NN] = unLocal[.., S];

    // swap all local arrays
    b.barrier();
    uLocal <=> unLocal;

    // compute the FD kernel in parallel
    foreach (i, j) in localIndicesInner do
      unLocal[i, j] = uLocal[i, j] + alpha * (
          uLocal[i-1, j] + uLocal[i, j-1] +
          uLocal[i+1, j] + uLocal[i, j+1] -
          4 * uLocal[i, j]
        );

    b.barrier();
  }

  // store results in global array
  uLocal <=> unLocal;
  u[myGlobalIndices] = uLocal[myGlobalIndices];
}
