// Test the standard module Random

use driver_real_arrays;
use Random;

config const printRefArrays = true;

var rng  = createRandomStream(314159265, eltType=real(64), algorithm=RNG.NPB);
var trng = createRandomStream(314159265, eltType=real(64), algorithm=RNG.NPB);

proc fillRefArrays() {
  rng.fillRandom(R1D);
  rng.fillRandom(R2D);
  rng.fillRandom(R3D);
  rng.fillRandom(R4D);
  rng.fillRandom(R2D32);
}

proc checkRNG(R, T: [?D2]) {
  var errs = 0;
  [i in D2 with (ref errs)] if R[i]!=T[i] {  // race => can miscount errors
    errs += 1;
    writeln(i, " does not match");
  }
  return errs;
}

var T1D: [Dom1D] real;
var T2D: [Dom2D] real;
var T3D: [Dom3D] real;
var T4D: [Dom4D] real;
var T2D32: [Dom2D32] real;

proc resetTempArrays() {
  [i in Dom1D] T1D[i] = -1;
  [ij in Dom2D] T2D[ij] = -1;
  [ijk in Dom3D] T3D[ijk] = -1;
  [ijkl in Dom4D] T4D[ijkl] = -1;
  [ij in Dom2D32] T2D32[ij] = -1;
}


writeln("fillRandom()");
resetTempArrays();
fillRefArrays();
if printRefArrays then
  outputRealArrays();
trng.fillRandom(T1D);
writeln("\tR1D: ", checkRNG(R1D, T1D), " errors");
trng.fillRandom(T2D);
writeln("\tR2D: ", checkRNG(R2D, T2D), " errors");
trng.fillRandom(T3D);
writeln("\tR3D: ", checkRNG(R3D, T3D), " errors");
trng.fillRandom(T4D);
writeln("\tR4D: ", checkRNG(R4D, T4D), " errors");
trng.fillRandom(T2D32);
writeln("\tR2D32: ", checkRNG(R2D32, T2D32), " errors");


writeln("fillRandom() aliased arrays");
ref aT1D = T1D;
ref aT2D = T2D;
ref aT3D = T3D;
ref aT4D = T4D;
ref aT2D32 = T2D32;
resetTempArrays();
fillRefArrays();
trng.fillRandom(aT1D);
writeln("\tR1D: ", checkRNG(R1D, aT1D), " errors");
trng.fillRandom(aT2D);
writeln("\tR2D: ", checkRNG(R2D, aT2D), " errors");
trng.fillRandom(aT3D);
writeln("\tR3D: ", checkRNG(R3D, aT3D), " errors");
trng.fillRandom(aT4D);
writeln("\tR4D: ", checkRNG(R4D, aT4D), " errors");
trng.fillRandom(aT2D32);
writeln("\tR2D32: ", checkRNG(R2D32, aT2D32), " errors");


writeln("fillRandom() reindexed arrays");
proc foo(rng, D: domain, A: [D]) {
  rng.fillRandom(A);
}
resetTempArrays();
fillRefArrays();
const TD1D: domain(1) = Space1.translate(-o5);
foo(trng, TD1D, T1D.reindex(TD1D));
writeln("\tR1D: ", checkRNG(R1D, T1D), " errors");
const TD2D: domain(2) = Space2.translate(-o5,-o5);
foo(trng, TD2D, T2D.reindex(TD2D));
writeln("\tR2D: ", checkRNG(R2D, T2D), " errors");
const TD3D: domain(3) = Space3.translate(-o5,-o5,-o5);
foo(trng, TD3D, T3D.reindex(TD3D));
writeln("\tR3D: ", checkRNG(R3D, T3D), " errors");
const TD4D: domain(4) = Space4.translate(-o5,-o5,-o5,-o5);
foo(trng, TD4D, T4D.reindex(TD4D));
writeln("\tR4D: ", checkRNG(R4D, T4D), " errors");
const TD2D32: domain(2,int(32)) = Space2D32.translate(-o5:int(32),-o5:int(32));
foo(trng, TD2D32, T2D32.reindex(TD2D32));
writeln("\tR2D32: ", checkRNG(R2D32, T2D32), " errors");


writeln("fillRandom() rank changed arrays");
ref rcT1D = T2D(Dom2D.dim(0), n2/2);
ref rcT2D = T3D(Dom3D.dim(0), Dom3D.dim(1), n3/2);
ref rcT3D = T4D(Dom4D.dim(0), Dom4D.dim(1), Dom4D.dim(2), n4/2);
const rcDom1D: domain(1) dmapped Dist1D = Dom2D.dim(0);
const rcDom2D: domain(2) dmapped Dist2D = {Dom3D.dim(0), Dom3D.dim(1)};
const rcDom3D: domain(3) dmapped Dist3D = {Dom4D.dim(0), Dom4D.dim(1), Dom4D.dim(2)};
var rcR1D: [rcDom1D] real;
var rcR2D: [rcDom2D] real;
var rcR3D: [rcDom3D] real;
resetTempArrays();
rng.fillRandom(rcR1D);
trng.fillRandom(rcT1D);
writeln("\trcR1D: ", checkRNG(rcR1D, rcT1D), " errors");
rng.fillRandom(rcR2D);
trng.fillRandom(rcT2D);
writeln("\trcR2D: ", checkRNG(rcR2D, rcT2D), " errors");
rng.fillRandom(rcR3D);
trng.fillRandom(rcT3D);
writeln("\trcR3D: ", checkRNG(rcR3D, rcT3D), " errors");

if printRefArrays then
  outputRealArrays();
