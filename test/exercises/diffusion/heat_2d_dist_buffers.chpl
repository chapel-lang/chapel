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
             solutionStd = 0.222751; // known solution for the default parameters

// define distributed domains and block-distributed array
const indices = {0..<nx, 0..<ny},
      indicesInner = indices.expand(-1),
      Indices = Block.createDomain(indices);

// define distributed 2D arrays over the above domain
var u, un: [Indices] real = 1.0;

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

  // define constants for indexing into edges of this tasks's region
  const wEdge = localIndices.dim(1).low,
        eEdge = localIndices.dim(1).high,
        sEdge = localIndices.dim(0).high,
        nEdge = localIndices.dim(0).low;

  // iterate for 'nt' time steps
  for 1..nt {
    // store results from last iteration in neighboring task's halo buffers
    if tidY > 0       then HaloArrays[tidX, tidY-1][E].v = u[.., wEdge];
    if tidY < tidYMax then HaloArrays[tidX, tidY+1][W].v = u[.., eEdge];
    if tidX > 0       then HaloArrays[tidX-1, tidY][S].v = u[nEdge, ..];
    if tidX < tidXMax then HaloArrays[tidX+1, tidY][N].v = u[sEdge, ..];

    // synchronize with other tasks
    b.barrier();

    // swap local arrays
    if tidX == 0 && tidY == 0 then u <=> un;

    // compute the FD kernel in parallel
    forall (i, j) in localIndicesInner.expand(-1) do
      u.localAccess[i, j] = un.localAccess[i, j] + alpha * (
        un.localAccess[i-1, j] + un.localAccess[i, j-1] +
        un.localAccess[i+1, j] + un.localAccess[i, j+1] -
        4 * un.localAccess[i, j]
      );

    // North
    if tidX > 0 {
      forall j in localIndicesInner.dim(1) do
        u.localAccess[nEdge, j] = un.localAccess[nEdge, j] + alpha * (
          HaloArrays[tidX, tidY][N].v[j] + un.localAccess[nEdge, j-1] +
          un.localAccess[nEdge+1, j]     + un.localAccess[nEdge, j+1] -
          4 * un.localAccess[nEdge, j]
        );
    }

    // South
    if tidX < tidXMax {
      forall j in localIndicesInner.dim(1) do
        u.localAccess[sEdge, j] = un.localAccess[sEdge, j] + alpha * (
          un.localAccess[sEdge-1, j]     + un.localAccess[sEdge, j-1] +
          HaloArrays[tidX, tidY][S].v[j] + un.localAccess[sEdge, j+1] -
          4 * un.localAccess[sEdge, j]
        );
    }

    // East
    if tidY < tidYMax {
      forall i in localIndicesInner.dim(0) do
        u.localAccess[i, eEdge] = un.localAccess[i, eEdge] + alpha * (
          un.localAccess[i-1, eEdge] + un.localAccess[i, eEdge-1] +
          un.localAccess[i+1, eEdge] + HaloArrays[tidX, tidY][E].v[i] -
          4 * un.localAccess[i, eEdge]
        );
    }

    // West
    if tidY > 0 {
      forall i in localIndicesInner.dim(0) do
        u.localAccess[i, wEdge] = un.localAccess[i, wEdge] + alpha * (
          un.localAccess[i-1, wEdge] + HaloArrays[tidX, tidY][W].v[i] +
          un.localAccess[i+1, wEdge] + un.localAccess[i, wEdge+1] -
          4 * un.localAccess[i, wEdge]
        );
    }

    b.barrier();
  }
}
