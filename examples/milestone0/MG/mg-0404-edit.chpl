-- problem size to run:

-- BLC: open which to use?
enum classVals {S, W, A, B, C, D, U};
type classVals = S | W | A | B | C | D | U;

-- #foo (or #"foo") -- globally unique symbol "#foo"
-- David: might use `foo

config const Class: classVals = S;    -- at top-level or module scopes only?  (static w/in fns?)
                                      -- suppress initializer evaluation if set on command line!

-- problem size parameters:

config const n: integer = setProbSize(Class);
config const nx: integer = n;
config const ny: integer = n;
config const nz: integer = n;

config const numLevels: integer = lg2(n); -- David: What if lg2 depends on an unset config/global?
                                          -- Q: order of evaluation of initializers

config const nit: integer = setNumIters(Class);



-- checksum:

config const verifyValue: float = setVerifyValue(Class);

config const (n, nit, verifyValue) = setClassParams(Class);
-- concretely:
-- config const (n: integer, nit: integer, verifyValue: float) = setClassParams(Class);

-- Q: What if someone wanted to override one of these, but not all three?

-- one proposal:
   - run config var initializer, assigning result to temp
   - then check if command line overrides; if so, assign result to temp
   - assign temp to config var




-- other control variables:

config const warmup: boolean = true;
config const verbose: boolean = false;
config const debug: boolean = false;


-- help for coefficient arrays:

type coeff = [0..3] float;

-- Q: are 0 and 3 part of the type of coeff?
-- should this be type coeff = [..] float?

-- Q: do we use structural or named type equivalence?


-- domains:

const Levels: domain(1) = (1..numLevels);
-- const Levels: domain(1) = 1..numLevels;
const Base: domain(3) distributed(block(3)) = [1..nx, 1..ny, 1..nz];
const Hier: [lvl: Levels] domain(Base) = Base by -2**(lvl-1);
const Stencil: domain(3) = [-1..1, -1..1, -1..1];

-- Given X:[Hier] float, I assume one can say X(1)(1, 1, 1)...       Brad
-- Q: Can one say X(1, 1, 1, 1)?  X(1, 1)(1, 1)?  X(1)(1)(1)(1) ?    no
-- Q: Can one do:  (a) X = 0.0;  (b) sum X; ?                        yes
-- Q: Can one do:  (a) sum(dim=2) X; ?                               no
-- Q: Is X a rank-4 thing or a rank-(1,3) thing?                     (1,3)
-- Q: Is X equivalent to Y: [lvl:Levels] [Hier(lvl)] float; ?        yes
-- Some things are maybe less reasonable:
-- Q: What about:  sum(dim=1) X; ?     -- what's the shape of this?
-- Q: X.reorder(4, 3, 2, 1); ???       -- how would we represent this?
-- Q: What is the "shape" of x (using the technical term)? -- not our standard beast

-- Entry point:

var initTimer: timer;
var benchTimer: timer;
-- Q: where are these implemented?
-- Q: semantics on multiple processors/locales?
-- Q: resolution of timers -- machine dependences...?
-- Q: code motion around timers? -- presumably illegal (put fence pragma in implementation?)


-- Q: Is it legal to have code at the top level like this?

initTimer.start();
  -- main arrays:
  var V: [Base] float;
  var U, R: [lvl: Levels] [Hier(lvl)] float;
-- or: var U, R: [Hier] float;
-- Q: how does this declaration of U and R interact with David's proposed tuple declaration
--    (x: typex, y: typey) = ...;?
-- John's brainstorming:
-- var x, y: xytype;  -- x and y are the same type (initializer illegal?  or tuple init allowed?)
-- For different types:
--    var (x: xtype, y: ytype) = (xval, yval);
-- OR var (x: xtype = xval, y:ytype = ybal);

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

function cfn(off: index(3)) {
  static const c: coeff = initCValues();

  return c(sum (off != 0));
  --     OR: (off(1) != 0) + (off(2) != 0) + (off(3) != 0));
  -- Q: We're mixing tuple and array notions here.  Is that OK?
}


function psinv(U, R) {
  static const c: coeff = initCValues();
  static const c3d: [(i,j,k): Stencil] float = c((i!=0) + (j!=0) + (k!=0));

  const RD = R.domain;
  const Rstr = RD.stride;

  U += [ijk:RD] sum [off:Stencil] (cfn(off) * R(ijk + Rstr*off));
}


function resid(R, V, U) {
  static const a: coeff = (-8.0/3.0, 0.0, 1.0/6.0, 1.0/12.0);
  static const a3d: [(i,j,k): Stencil] float = a((i!=0) + (j!=0) + (k!=0));

  const UD = U.domain;
  const Ustr = UD.stride;

  R = V - [ijk:UD] sum [off:Stencil] (a3d(off) * U(ijk + Ustr*off));
}


function rprj3(S, R) {
  static const w: coeff = (0.5, 0.25, 0.125, 0.0625);
  -- Q: Here's the tuple/array question again:  Introduce a unary operator that
  --    converts tuples to arrays? (e.g., #(0.5, ...) )  Or can we do it automatically?
  -- David: we probably need a primitive 1D vector type (dense in memory) anyway for
  --        layout, so maybe we just bite that bullet?
  -- Q: What is our vector/array literal syntax?
  static const w3d: [(i,j,k): Stencil] float = w((i!=0) + (j!=0) + (k!=0));

  const SD = S.domain;
  const Rstr = RD.stride;

  S = [ijk: SD] sum [off:Stencil] (w3d(off) * R(ijk + Rstr*off));
}


function interp(R, S) {
  static const IDom: domain(3) = [-1..0, -1..0, -1..0];
  static const IStn: [(i,j,k):IDom] domain(3) = [i..0, j..0, k..0];
  static const w: [ijk:IDom] float = 1.0 / IStn.size();

  const SD: S.domain;
  const Rstr: R.domain.stride;
  const Sstr: SD.stride;

  forall ioff in IDom {
    [ijk:SD] R(ijk + Rstr*ioff) 
               += w(ioff) * sum [off: IStn(ioff)] S(ijk + Sstr*off);
  }
  -- Q: What are performance implications here?
  -- Q: To what extent would this performance rely on unrolling, constant propagation?
  -- Q: Should user indicate that there would be some payoff to unrolling?
  -- Q: Goal-driven approach?  Is the inner loop worth unrolling?  If
  --    so, what would it take to get there?
  -- Q: What are idioms here?  Stencils... reductions over small loops
  -- (in order that we don't think of them as "innermost"...)
  -- Q: What does "small" mean?
}


-- Calculates approximate norms:

function norm2u3(R): (float, float) {
  const rnm2 = sqrt((sum R**2)/(nx*ny*nz));
  const rnmu = reduce by max abs(R);
  -- Q: User reduction, or rely on compiler to fuse loops?

  return (rnm2, rnmu);
}


-- Setup stuff

function initArrays(V, U, R) {
  -- conservatively, one might want to do "V=0.0; U=0.0; R=0.0; zran3(V);", 
  -- but the following is minimal:
  zran3(V);
  U(1) = 0.0;

  V.outOfBounds(wrap);
  forall lvl in Levels {
    U(lvl).outOfBounds(wrap);
    R(lvl).outOfBounds(wrap);
  }
  -- Q: What are the performance impacts of this?
  -- Q: If too expensive, how would we write this?
  -- Q: How would I put an outOfBounds function on the 1..numLevels dimension of U, R?
  -- Q: This is a dynamic property -- in order to inline/analyze statically, we'd have
  --    to know what the value is at any given point and may have to be conservative.
  --    should it be part of the type?  A single-assignment value?
  -- Q: Should this be a feature of the language (of arrays)?
  -- Q: Should there be a wraparray type instead?  (is wraparray user- or library-defined?)
  -- Q: Overload the access method for arrays to get same functionality?
  -- Q: Should/could we simply write a function (that acts as an array and implements
  --    the wraparound) and use that within the stencils instead?
}


function zran3(V) {
  const ncharge: integer = 10;
  var chargeloc: [1..ncharge] class {
                   var POS, NEG: index(Base);
                 };
  -- OR: var chargeloc: [1..ncharge] (index(Base), index(Base));  -- OK, but lost POS/Neg naming
  -- orig: var chargeloc: class { var POS, NEG: [1..ncharge] index(Base); }
  -- Q: what is this thing?  A structure declaration/record?  An anonymous class?
  -- BLC: note: chargeloc isn't really a required container here.  Just declare POS and NEG...

  V = [i,j,k: Base] longRandlc((i-1) + (j-1)*nx + (k-1)*nx*ny);

  -- BLC: would make sense to replace this with a user-defined reduction?
  for i in (1..ncharge) {
    chargeloc(i) = (reduce by maxloc V, reduce by minloc V);
    V(chargeloc(i).POS) = 0.0;
    V(chargeloc(i).NEG) = 0.0;
  }

  V = 0.0;
  for i in (1..ncharge) {
    V(chargeloc(i).POS) =  1.0;
    V(chargeloc(i).NEG) = -1.0;
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

function initCValues(Class): coeff {
  if (Class = 'A' || Class = 'S' || Class = 'W') {

  -- Q: Is this a type error?  I've stated I'm returning a [0..3] float, but am
  --    returning a 4-tuplex of floats.  Is that legal?

    return (-3.0/8.0,  1.0/32.0, -1.0/64.0, 0.0);

-- Q: John's array literal syntax:    return #[-3.0/8.0, 1.0/32.0, -1.0/64.0, 0.0];
  } else {
    return (-3.0/17.0, 1.0/33.0, -1.0/61.0, 0.0);
  }
-- Q: What are the interoperability requirements between tuples, 1D arrays, records, sequences?
}


-- functions mapping problem size to parameters:

function setProbSize(Class): integer {
  switch (Class) {
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


function setNumIters(Class): integer {
  switch (Class) {
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


function setVerifyValue(Class): float {
  switch (Class) {
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
