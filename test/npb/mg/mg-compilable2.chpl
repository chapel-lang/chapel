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

/*
class timer {
  proc start();
  proc stop();
  proc read() { return -1.0; }
}
*/

// an enumerated type indicating the standard NPB problem classes:
//   S: small
//   W: workstation
//   A-D: increasingly large "production" runs
//   O: other

enum classVals {S, W, A, B, C, D, O};


// arrays of values used to characterize each problem class:
//   probsize: the problem size per dimension of the finest grid
//   iterations: the number of iterations to run
//   checksum: the expected result, if the default characteristics are used

const probSize: [S..O] int = (32, 64, 256, 256, 512, 1024, 256);
const iterations: [S..O] int = (4, 40, 4, 20, 20, 50, 4);
const checksum: [S..O] real  = (0.0000530770700573,
                                  0.00000000000000000250391406439,
                                  0.000002433365309,
                                  0.00000180056440132,
                                  0.000000570674826298,
                                  0.000000000158327506043,
                                  0.0);


// this is the problem class that will be used for the run.  Since
// it's a configuration constant, it may be set on the executable's
// command-line.  The default is to run class S.

config const Class: classVals = classVals.S;


// the problem size.  By default, the problem size will be the same in
// each dimension (n), as specified by the Class variable.  This
// default may be overridden for all dimensions by specifying a new
// value for n on the executable's command-line or by overriding any
// of the dimensional problem sizes individually (nx, ny, nz).

config const n = probSize(Class);
config const nx = n,
             ny = n,
             nz = n;


// the number of levels in the multigrid hierarchy.  This is the log_2
// of the smallest dimension.  The user may specify a different number
// of levels on the executable's command-line.

config const numLevels = lg2(min(min(nx,ny),nz));


// the number of iterations to run.  Again, determined by the problem
// class by default, but overridable.

config const numIters = iterations(Class);


// the checksum for the benchmark, again overridable.

config const verifyValue = checksum(Class);


// A flag specifying whether or not a "warmup" round should be run, a
// single iteration should be run before the timing is started to warm
// up the cache (as in the NPB version).

config const warmup = true;


// a small, 4-element array type that we'll use to store the 4
// coefficient values that define the weight values used in each of
// the 27-point stencils.

type coeff = [0..3] real;


// the domains that define the multigrid data structure
//   Levels: a 1D arithmetic domain that defines the levels of the
//     hierarchy
//   Base: the base 3D arithmetic domain that defines the problem
//     size; this domain is distributed by blocking the three 
//     dimensions across the entire locale set using the standard 
//     "Block" distribution.

//   Hier: an array of 3D arithmetic domains, one for each level of
//     the hierarchy; each domain is a subdomain of "Base", and each is
//     strided by twice as much as the previous.  A negative stride is
//     used to start counting from the top of each dimension.  Thus, for
//     a 256^3 Base domain, the last domain would contain the single
//     index (256, 256, 256) rather than (1, 1, 1).  This is more
//     cosmetic than anything, and done to match the NAS version.
//   Stencil: a 3x3x3 domain used to describe the 27-point stencils
//     used in this computation

const Levels: domain(1) = {1..numLevels};
const Base: domain(3) = {1..nx, 1..ny, 1..nz};
//BLC: distributed doesn't work yet
//const Base: domain(3) dmapped(Block(3)) = {1..nx, 1..ny, 1..nz};
//BLC: iterators in array decls don't work yet
//const Hier: [lvl in Levels] subdomain(Base) = Base by -2**(lvl-1);
//BLC: subdomains don't work:
//var Hier: [Levels] subdomain(Base);
var Hier: [Levels] domain(3);
[lvl in Levels] Hier(lvl) = Base by -2**(lvl-1);
const Stencil: domain(3) = {-1..1, -1..1, -1..1};


// ENTRY POINT:

proc main() {
  // two timer variables that are used to time the initialization and
  // the benchmark time, respectively
  //  var initTimer = new timer(), benchTimer = timer();

  // Here's the timed initialization portion (indentation for emphasis)
  //  initTimer.start();

    // These are our main arrays; V is the input array; U and R are
    // the hierarchical arrays used in the computation
    var V: [Base] real;
    // BLC: iterators in array decls don't work yet
    //    var U, R: [lvl in Levels] [Hier(lvl)] real;
    var U, R: [Levels] [Base] real;

    // Initialize everything
    initializeMG(V, U, R);
    //  initTimer.stop();

  // Here's the actual benchmark run; rnm2 is our checksum value
    //  benchTimer.start();
    var rnm2 = computeMG(V, U, R);
    //  benchTimer.stop();

  // print out the results of the run and timings
    //  printResults(rnm2, initTimer.read(), benchTimer.read());
}


// TOP-LEVEL FUNCTIONS (called by main())


// initializeMG() gets things set up

proc initializeMG(V, U, R) {
  // print some standard heading information
  writeln(" NAS Parallel Benchmarks 3.0 (Chapel version) - MG Benchmark");
  writeln(" Size: ", nx, "x", ny, "x", nz);
  writeln(" Iterations: ", numIters);
  writeln();

  // warm up the cache if the user has requested it
  if (warmup) {
    initArrays(V, U, R);
    runOneIteration(V, U, R);
  }

  // initialize the arrays
  initArrays(V, U, R);
}


// computeMG() runs the computation, returning the computed checksum

proc computeMG(V, U, R) {
  resid(R(1), V, U(1));
  norm2u3(R(1));
  for it in 1..numIters {
    runOneIteration(V, U, R);
  }
  var rnm2 = norm2u3(R(1))(1);

  return rnm2;
}


// printResults() performs the checksum verification and prints out
// some general information about the run

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


// running one iteration means running a round of mg3P followed by
// one residual

proc runOneIteration(V, U, R) {
  mg3P(V, U, R);
  resid(R(1), V, U(1));
}


// mg3P() contains all the work needed to run a single multigrid cycle

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


// STENCIL ROUTINES: These are the guts of the multigrid computation;
// each applies a 27-point (3x3x3) stencil to an array to compute a
// modification to another array. resid() and psinv() conserve the
// problem size; rprj3() projects a finer array to a coarser one; and
// interp() does the opposite.  All four of these are reasonably
// similar, with some small differences.  I'll comment the first one
// well, and the others where they differ


// resid() applies a 27-point stencil to U, subtracts the result from
// V, and assigns to R.

proc resid(R, V, U) {
  // these are the weight values for the stencil
  const a: coeff = (-8.0/3.0, 0.0, 1.0/6.0, 1.0/12.0);

  // this sets up the 3x3x3 array of weights; the idea is that the
  // stencil element should get the 0th coefficient value; those
  // positions that are one coordinate off should get the 1st; two
  // coordinates off should get the 2nd and so on.  I wrote this by
  // doing some bool tests, and adding the results together,
  // relying on the implicit conversion of bools to ints.  My
  // hope is that by declaring these to be const, the compiler will do
  // stencil optimizations on them as defined in Deitz's SC2001 paper.

  //  const a3d: [(i,j,k) in Stencil] real = a((i!=0) + (j!=0) + (k!=0));
  var a3d: [Stencil] real;
  [i,j,k in Stencil] a3d(i,j,k) = a((i!=0) + (j!=0) + (k!=0));

  // grab the U array's domain and stride information, for
  // convenience.  The stride is returned as a triple of ints

  const UD = U.Domain,
        Ustr = U.stride;

  // The square brackets here are a forall expressions, used to
  // iterate over a domain, naming the index if desired.  So this
  // iterates over all of the indices in U's domain (calling them
  // ijk), performing for each index a sum reduction over the indices
  // in the 27-point Stencil domain.  The elements that are reduced
  // are the product of the corresponding stencil weight (a3d(off))
  // and the U array element corresponding to the original position
  // (U(ijk)), offset by the stencil index multiplied by the offset
  // elementwise (Ustr*off).

  // This array of values is subtracted from the V array and assigned
  // to the R array.

  R = V - [ijk in UD] + reduce [off in Stencil] 
                                 (a3d(off) * U(ijk + Ustr*off));
}


// psinv() is almost identical to psinv().  It computes the stencil on
// the R array, accumulating the result into the U array.  The main
// difference is that its weight array varies depending on the problem
// size for some reason (maybe there was a bug in the early version of
// the benchmark that has been preserved for backwards compatibility?)

proc psinv(U, R) {
  const c: coeff = initCValues();
  //  const c3d: [(i,j,k) in Stencil] real = c((i!=0) + (j!=0) + (k!=0));
  var c3d: [Stencil] real;
  [i,j,k in Stencil] c3d(i,j,k) = c((i!=0) + (j!=0) + (k!=0));

  const RD = R.Domain,
        Rstr = R.stride;

  U += [ijk in RD] + reduce [off in Stencil] (c3d(off) * R(ijk + Rstr*off));
}


// rprj3() is very similar except that the R array is finer than the S
// array, so the main loop iterates over the domain of S rather than
// R.

proc rprj3(S, R) {
  const w: coeff = (0.5, 0.25, 0.125, 0.0625);
  //  const w3d: [(i,j,k) in Stencil] real = w((i!=0) + (j!=0) + (k!=0));
  var w3d: [Stencil] real;
  [i,j,k in Stencil] w3d(i,j,k) = w((i!=0) + (j!=0) + (k!=0));

  const SD = new S.Domain,
        Rstr = R.stride;

  S = [ijk in SD] + reduce [off in Stencil] (w3d(off) * R(ijk + Rstr*off));
}


// the interp() routines is slightly more complicated because for each
// point being interpolated, there are one of eight possible
// topologies for the points being sampled (ranging from being
// directly aligned, in which case the point is simply used, to being
// completely unaligned in which case the 8 neighboring points are
// averaged).

// I represented this here by creating a domain IDom that represents
// the 8 topologies (numbered -1..0 for each dimension, since we've
// aligned the coarser domains with the higher indices); IStn is then
// an array of stencil domains, like Stencil above, except that each
// offset range is either -1..0 or 0..0, depending on the topology.
// The "w" array stores the weights for each of the stencils, which is
// simply 1.0 dividedby the number of elements in the stencil (1 - 8).

// the computation proceeds very similarly to the previous stencils,
// except that we perform the eight topologies in parallel.  We also
// iterate over the coarser (S) array's domain, and apply offsets to
// the finer (R) array offsets in order to get the different
// topologies.

proc interp(R, S) {
  const IDom: domain(3) = {-1..0, -1..0, -1..0};
  //  const IStn: [(i,j,k) in IDom] domain(3) = {i..0, j..0, k..0};
  var IStn: [IDom] domain(3);
  [i,j,k in IDom] Istn(i,j,k) = {i..0, j..0, k..0};
  //  const w: [ijk in IDom] real = 1.0 / IStn.size();
  var w: [IDom] real;
  [ijk in IDom] w(ijk) = 1.0 / IStn.size();

  const SD = new S.Domain(),
        Rstr = R.stride,
        Sstr = new S.stride;

  forall ioff in IDom {
    [ijk in SD] R(ijk + Rstr*ioff) 
                = w(ioff) * + reduce [off in IStn(ioff)] S(ijk + Sstr*off);
  }
}


// norm2u3() computes approximate norms from an array using sum and
// max reductions.  It returns a 2-tuple of values (only one of which
// is used in this benchmark // the other is dropped on the floor at
// the callsite).

proc norm2u3(R) {
  const rnm2 = sqrt((+ reduce R**2)/(nx*ny*nz)),
        rnmu = max reduce abs(R);

  return (rnm2, rnmu);
}


// initCValues() sets the c values for the psinv() stencil

proc initCValues(Class) {
  if (Class == classVals.A || Class == classVals.S || Class == classVals.W) {
    return (-3.0/8.0,  1.0/32.0, -1.0/64.0, 0.0);
  } else {
    return (-3.0/17.0, 1.0/33.0, -1.0/61.0, 0.0);
  }
}



// SETUP ROUTINES:

// initArrays() initializes the arrays used in the computation.

proc initArrays(V, U, R) {
  // conservatively, one might want to do "V=0.0; U=0.0; R=0.0; zran3(V);", 
  // but the following is minimal:
  zran3(V);
  U(1) = 0.0;

  // this is the most speculative part of this implementation; I've
  // proposed support in Chapel for supporting boundary conditions by
  // associating funs with arrays that specify what should be
  // done when an array reference is out-of-bounds.  Some basic,
  // provided funs might be to declare an error, to return the
  // zero for the array's element type, to wrap the reference around
  // to the opposite side of the array, or to reflect the reference
  // back into the array.  Because all of the 27-point stencils in
  // this benchmark need to be applied using a 3D toroidal wrap, I
  // specify this here using a method on the array.

  // If such a mechanism was not provided, the user would have to
  // explicitly declare extra storage for the boundaries and copy the
  // boundary cells around, which is both tedious, and less likely to
  // result in the systolic communication pattern that the stencils
  // require.  I argue that a mechanism like the following is not that
  // much more onerous than doing the local per-processor boundaries
  // that the compiler and runtime already have to manage, and that it
  // therefore imposes similar requirements and challenges.  I can
  // expound more on this in person than is worth embedding here.

  //  V.setBoundaryCondition(wrap);
  //  forall lvl in Levels {
  //    U(lvl).setBoundaryCondition(wrap);
  //    R(lvl).setBoundaryCondition(wrap);
  //  }
}


// zran3() computes a random array of values, finds the 10 largest and
// smallest values, and sets these values to 1.0 and -1.0 respectively
// in our input array, setting the rest to 0.0.  It's for this reason
// that one mightwant to store the input array as a sparse array,
// though this implementation didn't take that approach (a sparse
// version could be written on request // if we did our job right,
// just the declaration of V would change)

proc zran3(V) {
  const numCharges = 10;
  var pos, neg: [1..numCharges] index(Base);

  // compute the random array of values
  V = [i,j,k in Base] longRandlc((i-1) + (j-1)*nx + (k-1)*nx*ny);

  // the following does a number of maxloc/minloc reductions, each of
  // which returns the location of the largest/smallest element as an
  // index of the array's domain.  This is an easy, though slow way to
  // perform this computation.  A better way would be to write a
  // user-defined reduction using Chapel's features for that, but this
  // required lots more effort and it isn't being timed anyway.

  for i in (1..numCharges) {
    pos(i) = maxloc reduce V;
    neg(i) = minloc reduce V;
    pos(i) = 0.5;  // (remove from consideration on next iteration)
    neg(i) = 0.5;
  }

  // zero out the array and assign the initial 1.0 and -1.0 values
  V = 0.0;
  for i in (1..numCharges) {
    V(pos(i)) =  1.0;
    V(neg(i)) = -1.0;
  }
}


// "RANDOM" NUMBER STUFF: This is basically the same as what the NPB
// supplies in randdp.f; I don't claim to understand them in any
// depth.

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


// simple math helper fun; perhaps this will be part of the
// standard library, but it was easy enough to write

proc lg2(x) {
  var lg = -1;
  while (x) {
    x *= 2;
    lg += 1;
  }
  return lg;
}
