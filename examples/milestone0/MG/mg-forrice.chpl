-- problem size to run:

enum classVals = S | W | A | B | C | D | U;

config const class: classVals = S;


-- problem size parameters:

config const n: integer = setProbSize(class);
config const nx: integer = n;
config const ny: integer = n;
config const nz: integer = n;

config const numLevels: integer = lg2(n);

config const nit: integer = setNumIters(class);


-- checksum:

config const verifyValue: float = setVerifyValue(class);


-- other control variables (some unused for now):

config const warmup: boolean = true;
config const verbose: boolean = false;
config const debug: boolean = false;


-- help for coefficient arrays:

type coeff = [0..3] float;


-- domains:

const Levels: domain(1) = (1..numLevels);
const Base: domain(3) distributed(block(3)) = [1..nx, 1..ny, 1..nz];
const Hier: [lvl: Levels] domain(Base) = Base by -2**(lvl-1);
const Stencil: domain(3) = [-1..1, -1..1, -1..1];


-- timers

var initTimer: timer;
var benchTimer: timer;


-- code starts here

initTimer.start();
  -- main arrays:
  var V: [Base] float;
  var U, R: [lvl: Levels] [Hier(lvl)] float;

  initializeMG();
  inittime = warmupMG(V, U, R);
initTimer.stop();

benchTimer.start();
  var rnm2: float = computeMG(V, U, R);
benchTimer.stop();

printResults(rnm2, initTimer.read(), benchTimer.read());


-- Top-level functions:

function initializeMG(V, U, R) {
  writeln(" NAS Parallel Benchmarks 2.4 (Chapel version) - MG Benchmark");
  writeln(" Size: ", nx, "x", ny, "x", nz);
  writeln(" Iterations: ", nit);
  writeln();

  initArrays(V, U, R);
}


function warmupMG(V, U, R) {
  if (warmup) {
    mg3P(V, U, R);
    resid(R(1), V, U(1));

    initArrays(V, U, R);
  }
}


function computeMG(V, U, R): float {
  resid(R(1), V, U(1));
  norm2u3(R(1));
  for it in (1..nit) {
    mg3P(V, U, R);
    resid(R(1), V, U(1));
  }
  rnm2 = norm2u3(R(1))(1);

  return rnm2;
}


function printResults(rnm2: float; inittime, runtime: float) {
  var verified: boolean;

  if (verifyValue != 0.0) {
    const epsilon: float = 1.e-8;
    const delta: float = rnm2 - verifyValue;

    if (abs(delta) <= epsilon) {
      writeln(" VERIFICATION SUCCESSFUL ");
      writeln(" L2 Norm is ", rnm2);
      writeln(" Error is ", delta);
    } else {
      writeln(" VERIFICATION FAILED");
      writeln(" L2 norm is ", rnm2);
      writeln(" The correct L2 Norm is ", verifyValue);
    }
  } else {
    verified = false;
    writeln(" Problem size unknown");
    writeln(" NO VERIFICATION PERFORMED");
  }
  writeln(" MG Benchmark Completed.");
  writeln(" Size = ", nx, "x", ny, "x", nz);
  writeln(" Iterations = ", nit);
  writeln(" Time in seconds = ", runtime);
  writeln(" Mop/s total = ???");
  writeln(" Operation type = floating point");
  write("Verification = ");
  if (verified) {
    writeln("   SUCCESSFUL");
  } else {
    writeln(" UNSUCCESSFUL");
  }
  writeln(" Version = 2.4");
}


-- Work for a single iteration:

function mg3P(V, U, R) {
  -- project up the hierarchy
  for lvl in (2..numLevels) {
    rprj3(R(lvl), R(lvl-1));
  }

  -- compute at the top of the hierarchy
  U(numLevels) = 0.0;
  psinv(U(numLevels), R(numLevels));

  -- interpolate down the hierarchy
  for lvl in (2..numLevels-1) by -1 {
    U(lvl) = 0.0;
    interp(U(lvl), U(lvl+1));
    resid(R(lvl), R(lvl), U(lvl));
    psinv(U(lvl), R(lvl));
  }

  -- compute at the bottom of the hierarchy
  interp(U(1), U(2));
  resid(R(1), V, U(1));
  psinv(U(1), R(1));
}


-- Here's the meat: the four stencil routines:

function psinv(U, R) {
  static const c: coeff = initCValues();
  static const c3d: [(i,j,k): Stencil] float = c((i!=0) + (j!=0) + (k!=0));

  const RD = R.domain;
  const Rstr = R.stride;

  U += [ijk:RD] sum [off:Stencil] (c3d * R(ijk + Rstr*off));
}


function resid(R, V, U) {
  static const a: coeff = (-8.0/3.0, 0.0, 1.0/6.0, 1.0/12.0);
  static const a3d: [(i,j,k): Stencil] float = a((i!=0) + (j!=0) + (k!=0));

  const UD = U.domain;
  const Ustr = U.stride;

  R = V - [ijk:UD] sum [off:Stencil] (a3d * U(ijk + Ustr*off));
}


function rprj3(S, R) {
  static const w: coeff = (0.5, 0.25, 0.125, 0.0625);
  static const w3d: [(i,j,k): Stencil] float = w((i!=0) + (j!=0) + (k!=0));

  const RD = R.domain;
  const Rstr = R.stride;

  S = [ijk: RD] sum [off:Stencil] (w3d * R(ijk + Rstr*off));
}


function interp(R, S) {
  static const IDom: domain(3) = [-1..0, -1..0, -1..0];
  static const IStn: [(i,j,k):IDom] domain(3) = [i..0, j..0, k..0];
  static const w: [ijk:IDom] float = 1.0 / IStn.size();

  const SD: S.domain();
  const Rstr: R.stride;
  const Sstr: S.stride;

  forall ioff in IDom {
    [ijk:SD] R(ijk + Rstr*ioff) 
               += w(ioff) * sum [off: IStn(ioff)] S(ijk + Sstr*off);
  }
}


-- Calculates approximate norms:

function norm2u3(R): (float, float) {
  const rnm2 = sqrt((sum R**2)/(nx*ny*nz));
  const rnmu = reduce by max abs(R);

  return (rnm2, rnmu);
}


-- Setup stuff

function initArrays(V, U, R) {
  -- conservatively, one might want to do "V=0.0; U=0.0; R=0.0; zran3(V);", 
  -- but the following is minimal:
  zran3(V);
  U(1) = 0.0;

  V.outOfBounds(wrap);
  U.outOfBounds(wrap);
  R.outOfBounds(wrap);
}


function zran3(V) {
  const ncharge: integer = 10;
  var chargeloc: record
                   POS, NEG: (1..ncharge) of index(Base);
                 end;

  V = [i,j,k: Base] longRandlc((i-1) + (j-1)*nx + (k-1)*nx*ny);

  -- BLC: would make sense to replace this with a user-defined reduction
  for i in (1..ncharge) {
    var chargeloc.POS(i) = reduce by maxloc V;
    var chargelog.NEG(i) = reduce by minloc V;
  }

  V = 0.0;
  for i in (1..ncharge) {
    V(chargeloc.POS(i)) =  1.0;
    V(chargeloc.NEG(i)) = -1.0;
  }
}


-- random number generators

function longRandlc(n:integer): float {
  static const s: float = 314159265.0;
  static const arand: float = 5.0**13;

  var kk: integer = n;
  var t1: float = s;
  var t2: float = arand;
  var t3: float;

  while (kk != 0) {
    var ik: integer = kk / 2;
    if (2*ij != kk) {
      t3 = randlc(t1, t2);
    }
    if (kk > 0) {
      t3 = randlc(t2, t2);
    }
    kk = k1;
  }

  return randlc(t1, arand);
}


function randlc(inout x: float; a: float): float {
  static const r23: float = 0.5**23;
  static const t23: float = 2**23;
  static const r46: float = 0.5**46;
  static const t46: float = 2**46;

  var t1, t2, t3, t4, a1, a2, x1, x2, y: float;

  t1 = r23 * x;
  a1 = integer(t1);
  a2 = a - t23 * a1;

  t1 = r23 * x;
  x1 = integer(t1);
  x2 = x - t23 * x1;
  t1 = a1 * x2 + a2 * x1;
  t2 = integer(r23 * t1);
  y = t1 - t23 * t2;
  t3 = t23 * y + a2 * x2;
  t3 = integer(r46 * t3);
  x = t3 - t46 * t4;

  return r46 * x;
}


-- help with coefficients

function initCValues(class): coeff {
  if (class = 'A' || class = 'S' || class = 'W') {
    return (-3.0/8.0,  1.0/32.0, -1.0/64.0, 0.0);
  } else {
    return (-3.0/17.0, 1.0/33.0, -1.0/61.0, 0.0);
  }
}


-- functions mapping problem size to parameters:

function setProbSize(class): integer {
  switch (class) {
  case 'S':
    return 32;
  case 'W':
    return 64;
  case 'A':
  case 'B':
    return 256;
  case 'C':
    return 512;
  default:
    return 256;
  }
}


function setNumIters(class): integer {
  switch (class) {
  case 'S':
  case 'A':
    return 4;
  case 'B':
  case 'C':
    return 20;
  case 'W':
    return 40;
  default:
    return 4;
  }
}


function setVerifyValue(class): float {
  switch (class) {
  case 'S':
    return 0.0000530770700573;
  case 'W':
    return 0.00000000000000000250391406439;
  case 'A':
    return 0.000002433365309;
  case 'B':
    return 0.00000180056440132;
  case 'C':
    return 0.000000570674826298;
  default:
    return 0.0;
  }
}


-- simple math helper function:

function lg2(x): integer {
  var lg: integer = -1;
  while (x) {
    x >>= 1;
    lg += 1;
  }
  return lg;
}
