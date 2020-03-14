// This is a port of the NPB MG benchmark (version 3.0) written in
// Chapel by Brad Chamberlain.  As with any port, a number of
// structural and stylistic choices were made; in some cases, the
// original identifiers were preserved, either for clarity, or because
// the author couldn't come up with better names for them.  In other
// cases, more verbose names were used in hopes of improving
// readability.

// In general, types are elided from declarations unless they are
// believed to be ambiguous when reading the source, or greatly
// improve the code's readability.  This is obviously a judgement
// call.

// It should be noted that as of this writing (Nov 2005), our
// prototype Chapel implementation is not yet far enough along to
// handle this code.  This therefore represents the target that we are
// striving for rather than something that works today.

enum classVals {S, W, A, B, C, D, O};


const probSize: [S..O] int = (32, 64, 256, 256, 512, 1024, 256),
      iterations: [S..O] int = (4, 40, 4, 20, 20, 50, 4),
      checksum: [S..O] real  = (0.0000530770700573,
                                  0.00000000000000000250391406439,
                                  0.000002433365309,
                                  0.00000180056440132,
                                  0.000000570674826298,
                                  0.000000000158327506043,
                                  0.0);


config const Class: classVals = classVals.S;


config const n = probSize(Class);
config const nx = n,
             ny = n,
             nz = n;


config const numLevels = log2(min(nx,ny,nz)),
             numIters = iterations(Class),
             verifyValue = checksum(Class);

config const warmup = true;

type coeff = [0..3] real;

const Levels: domain(1) = {1..numLevels};
const Base: domain(3) dmapped(Block(3)) = {1..nx, 1..ny, 1..nz};
const Hier: [lvl in Levels] domain(Base) = Base by -2**(lvl-1);
const Stencil: domain(3) = {-1..1, -1..1, -1..1};


proc main() {
  var initTimer, benchTimer: timer;

  initTimer.start();
    var V: [Base] real;
    var U, R: [lvl in Levels] [Hier(lvl)] real;

    initializeMG();
  initTimer.stop();

  benchTimer.start();
    var rnm2 = computeMG(V, U, R);
  benchTimer.stop();

  printResults(rnm2, initTimer.read(), benchTimer.read());
}


// top-level funs

proc initializeMG(V, U, R) {
  writeln(" NAS Parallel Benchmarks 3.0 (Chapel version) - MG Benchmark");
  writeln(" Size: ", nx, "x", ny, "x", nz);
  writeln(" Iterations: ", numIters);
  writeln();

  if (warmup) {
    initArrays(V, U, R);
    runOneIteration(V, U, R);
  }

  initArrays(V, U, R);
}


proc computeMG(V, U, R) {
  resid(R(1), V, U(1));
  norm2u3(R(1));
  for it in (1..numIters) {
    runOneIteration(V, U, R);
  }
  var (rnm2, _) = norm2u3(R(1));

  return rnm2;
}


proc printResults(rnm2, inittime, runtime) {
  var verified = false;

  if (verifyValue != 0.0) {
    const epsilon = 0.00000001,
          delta = rnm2 - verifyValue;

    if (abs(delta) <= epsilon) {
      writeln(" VERIFICATION SUCCESSFUL ");
      writeln(" L2 Norm is ", rnm2);
      writeln(" Error is ", delta);
      verified = true;
    } else {
      writeln(" VERIFICATION FAILED");
      writeln(" L2 norm is ", rnm2);
      writeln(" The correct L2 Norm is ", verifyValue);
    }
  } else {
    writeln(" Problem size unknown");
    writeln(" NO VERIFICATION PERFORMED");
  }
  writeln(" MG Benchmark Completed.");
  writeln(" Size = ", nx, "x", ny, "x", nz);
  writeln(" Iterations = ", numIters);
  writeln(" Time in seconds = ", runtime);
  write("Verification = ");
  if (verified) {
    writeln("   SUCCESSFUL");
  } else {
    writeln(" UNSUCCESSFUL");
  }
  writeln(" Version = 3.0");
}


proc runOneIteration(V, U, R) {
  mg3P(V, U, R);
  resid(R(1), V, U(1));
}


proc mg3P(V, U, R) {
  // project up the hierarchy
  for lvl in (2..numLevels) {
    rprj3(R(lvl), R(lvl-1));
  }

  // compute at the top of the hierarchy
  U(numLevels) = 0.0;
  psinv(U(numLevels), R(numLevels));

  // interpolate down the hierarchy
  for lvl in (2..numLevels-1) by -1 {
    U(lvl) = 0.0;
    interp(U(lvl), U(lvl+1));
    resid(R(lvl), R(lvl), U(lvl));
    psinv(U(lvl), R(lvl));
  }

  // compute at the bottom of the hierarchy
  interp(U(1), U(2));
  resid(R(1), V, U(1));
  psinv(U(1), R(1));
}


proc resid(R, V, U) {
  const a: coeff = (-8.0/3.0, 0.0, 1.0/6.0, 1.0/12.0);
  const a3d: [(i,j,k) in Stencil] real = a((i!=0) + (j!=0) + (k!=0));

  const UD = U.Domain,
        Ustr = U.stride;

  R = V - [ijk in UD] sum reduce [off in Stencil] 
                                   (a3d(off) * U(ijk + Ustr*off));
}


proc psinv(U, R) {
  const c: coeff = initCValues();
  const c3d: [(i,j,k) in Stencil] real = c((i!=0) + (j!=0) + (k!=0));

  const RD = R.Domain,
        Rstr = R.stride;

  U += [ijk in RD] sum reduce [off in Stencil] (c3d(off) * R(ijk + Rstr*off));
}


proc rprj3(S, R) {
  const w: coeff = (0.5, 0.25, 0.125, 0.0625);
  const w3d: [(i,j,k) in Stencil] real = w((i!=0) + (j!=0) + (k!=0));

  const SD = S.Domain,
        Rstr = R.stride;

  S = [ijk in SD] sum reduce [off in Stencil] (w3d(off) * R(ijk + Rstr*off));
}


proc interp(R, S) {
  const IDom: domain(3) = {-1..0, -1..0, -1..0};
  const IStn: [(i,j,k) in IDom] domain(3) = {i..0, j..0, k..0};
  const w: [ijk in IDom] real = 1.0 / IStn.size();

  const SD = S.Domain(),
        Rstr = R.stride,
        Sstr = S.stride;

  forall ioff in IDom {
    [ijk in SD] R(ijk + Rstr*ioff) 
                = w(ioff) * sum reduce [off in IStn(ioff)] S(ijk + Sstr*off);
  }
}


proc norm2u3(R) {
  const rnm2 = sqrt((sum reduce R**2)/(nx*ny*nz)),
        rnmu = max reduce abs(R);

  return (rnm2, rnmu);
}


proc initCValues(Class) {
  if (Class == A || Class == S || Class == W) {
    return (-3.0/8.0,  1.0/32.0, -1.0/64.0, 0.0);
  } else {
    return (-3.0/17.0, 1.0/33.0, -1.0/61.0, 0.0);
  }
}



// SETUP ROUTINES:

proc initArrays(V, U, R) {
  // conservatively, one might want to do "V=0.0; U=0.0; R=0.0; zran3(V);", 
  // but the following is minimal:
  zran3(V);
  U(1) = 0.0;

  V.setBoundaryCondition(wrap);
  forall lvl in Levels {
    U(lvl).setBoundaryCondition(wrap);
    R(lvl).setBoundaryCondition(wrap);
  }
}


proc zran3(V) {
  const numCharges = 10;
  var pos, neg: [1..numCharges] index(Base);

  V = [i,j,k in Base] longRandlc((i-1) + (j-1)*nx + (k-1)*nx*ny);

  // the following does a number of maxloc/minloc reductions, each of
  // which returns the location of the largest/smallest element as an
  // index of the array's domain.  This is an easy, though slow way to
  // perform this computation.  A better way would be to write a
  // user-defined reduction using Chapel's features for that, but this
  // required more effort and it isn't being timed anyway.

  for i in (1..numCharges) {
    pos(i) = maxloc reduce V;
    neg(i) = minloc reduce V;
    pos(i) = 0.5;  // (remove from consideration on next iteration)
    neg(i) = 0.5;
  }

  V = 0.0;
  for i in (1..numCharges) {
    V(pos(i)) =  1.0;
    V(neg(i)) = -1.0;
  }
}


proc longRandlc(n) {
  const s = 314159265.0,
        arand = 5.0**13;

  var kk = n,
      t1 = s,
      t2 = arand,
      t3: real;

  while (kk != 0) {
    var ik = kk / 2;
    if (2*ik != kk) {
      t3 = randlc(t1, t2);
    }
    if (kk > 0) {
      t3 = randlc(t2, t2);
    }
    kk = ik;
  }

  return randlc(t1, arand);
}


proc randlc(x, a) {
  const r23 = 0.5**23,
        t23 = 2.0**23,
        r46 = 0.5**46,
        t46 = 2.0**46;

  var t1, t2, t3, t4, a1, a2, x1, x2, y: real;

  t1 = r23 * x;
  a1 = t1:int;
  a2 = a - t23 * a1;

  t1 = r23 * x;
  x1 = t1:int;
  x2 = x - t23 * x1;
  t1 = a1 * x2 + a2 * x1;
  t2 = (r23 * t1):int;
  y = t1 - t23 * t2;
  t3 = t23 * y + a2 * x2;
  t3 = (r46 * t3):int;
  x = t3 - t46 * t4;

  return r46 * x;
}
