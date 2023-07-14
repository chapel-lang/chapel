import BlockDist.Block,
       Collectives.barrier;

use CommDiagnostics;
config param runCommDiag = false;

config const xLen = 2.0,    // length of the grid in x
             yLen = 2.0,    // length of the grid in y
             nx = 31,       // number of grid points in x
             ny = 31,       // number of grid points in y
             nt = 50,       // number of time steps
             sigma = 0.25,  // stability parameter
             nu = 0.05;     // viscosity

const dx : real = xLen / (nx - 1),       // grid spacing in x
      dy : real = yLen / (ny - 1),       // grid spacing in y
      dt : real = sigma * dx * dy / nu;  // time step size

// define block distributed array
const indices = {0..<nx, 0..<ny},
      indicesInner = indices.expand(-1),
      INDICES = Block.createDomain(indices);
var u: [INDICES] real;

// apply initial conditions
u = 1.0;
u[
  (0.5 / dx):int..<(1.0 / dx + 1):int,
  (0.5 / dy):int..<(1.0 / dy + 1):int
] = 2;

// ghost vector type for creating a "skyline" array of buffers
record GhostVec {
  var d: domain(1);
  var v: [d] real;

  proc init() do this.d = {0..0};
  proc init(r: range(int, boundKind.both, strideKind.one)) do
    this.d = {r};
}

// set up array of ghost vectors over same distribution as 'u'
var TL_DOM = Block.createDomain(u.targetLocales().domain);
var ghostVecs: [TL_DOM] [0..<4] GhostVec;

// numbers for indexing into local array edges
param N = 0, S = 1, E = 2, W = 3;

// number of tasks per dimension based on Block distributions decomposition
const tidXMax = u.targetLocales().dim(0).high,
      tidYMax = u.targetLocales().dim(1).high;

// barrier for one-task-per-locale
var b = new barrier(numLocales);

proc main() {
  if runCommDiag then startCommDiagnostics();

  // execute the FD compuation with one task per locale
  coforall (loc, (tidX, tidY)) in zip(u.targetLocales(), u.targetLocales().domain) do on loc {
    // initialize ghost vectors
    ghostVecs[tidX, tidY][N] = new GhostVec(u.localSubdomain().dim(1).expand(1));
    ghostVecs[tidX, tidY][S] = new GhostVec(u.localSubdomain().dim(1).expand(1));
    ghostVecs[tidX, tidY][E] = new GhostVec(u.localSubdomain().dim(0).expand(1));
    ghostVecs[tidX, tidY][W] = new GhostVec(u.localSubdomain().dim(0).expand(1));

    // synchronize across tasks
    b.barrier();

    // run the portion of the FD computation owned by this task
    work(tidX, tidY);
  }

  if runCommDiag {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
  }

  // print final results
  const mean = (+ reduce u) / u.size,
        stdDev = sqrt((+ reduce (u - mean)**2) / u.size);

  writeln(abs(0.102424 - stdDev) < 1e-6);
}

proc work(tidX: int, tidY: int) {
  // declare two local sub-arrays with room to store neighboring locale's edges
  const localIndices = u.localSubdomain(here),
        localIndicesBuffered = localIndices.expand(1),
        localIndicesInner = localIndices[indicesInner];

  var uLocal1, uLocal2: [localIndicesBuffered] real = 1;

  // populate local arrays with initial conditions from global array
  uLocal1[localIndices] = u[localIndices];
  uLocal2 = uLocal1;

  // convenient constants for indexing into edges of local array
  const WW = localIndicesBuffered.dim(1).low,
        EE = localIndicesBuffered.dim(1).high,
        SS = localIndicesBuffered.dim(0).high,
        NN = localIndicesBuffered.dim(0).low;

  // run FD computation
  for 1..nt {
    // store results from last iteration in neighboring tasks buffers
    if tidY > 0       then ghostVecs[tidX, tidY-1][E].v = uLocal2[.., WW+1];
    if tidY < tidYMax then ghostVecs[tidX, tidY+1][W].v = uLocal2[.., EE-1];
    if tidX > 0       then ghostVecs[tidX-1, tidY][S].v = uLocal2[NN+1, ..];
    if tidX < tidXMax then ghostVecs[tidX+1, tidY][N].v = uLocal2[SS-1, ..];

    // swap arrays
    b.barrier();
    uLocal1 <=> uLocal2;

    // populate local edges from ghost regions
    if tidY > 0       then uLocal1[.., WW] = ghostVecs[tidX, tidY][W].v;
    if tidY < tidYMax then uLocal1[.., EE] = ghostVecs[tidX, tidY][E].v;
    if tidX > 0       then uLocal1[NN, ..] = ghostVecs[tidX, tidY][N].v;
    if tidX < tidXMax then uLocal1[SS, ..] = ghostVecs[tidX, tidY][S].v;

    // compute the FD kernel in parallel
    foreach (i, j) in localIndicesInner do
      uLocal2[i, j] = uLocal1[i, j] +
              nu * dt / dy**2 *
                (uLocal1[i-1, j] - 2 * uLocal1[i, j] + uLocal1[i+1, j]) +
              nu * dt / dx**2 *
                (uLocal1[i, j-1] - 2 * uLocal1[i, j] + uLocal1[i, j+1]);

    b.barrier();
  }

  // store results in global array
  uLocal1 <=> uLocal2;
  u[localIndices] = uLocal1[localIndices];
}
