//
// This test has a series of functions that compute a dot product of two vectors.
//

use Time only stopwatch;
use Random only randomStream;
use CTypes only c_array;


config const N = 10_000;
config const iterations = 50;

config type elementType = real(64);
config param unrollFactor = 4;

config const seed = 314159;
config const minRand: elementType = -100;
config const maxRand: elementType = 100;
config const correctness = false;

assert(N % unrollFactor == 0, "N must be a multiple of unrollFactor");
compilerAssert(unrollFactor == 2 || unrollFactor == 4 || unrollFactor == 8,
               "unrollFactor must be 2, 4, or 8");

// Testing a simple serial for dot product
proc dotProdFor(n: int, x: [1..n] elementType, y: x.type): elementType {
  var s: elementType;
  for i in 1..n do s += x[i] * y[i];
  return s;
}
// Same as `dotProdFor`, but using a foreach loop
proc dotProdForeach(n: int, x: [1..n] elementType, y: x.type): elementType {
  var s: elementType;
  foreach i in 1..n with (ref s) do s += x[i] * y[i];
  return s;
}
// The Chapeltastic approach, using high level Chapel features
proc dotProdChapeltastic(n: int, x: [1..n] elementType, y: x.type): elementType {
  var s: elementType = + reduce (x * y);
  return s;
}
// An unrolled version of the dot product, using slices
proc dotProdSlices(n: int, x: [1..n] elementType, y: x.type): elementType {
  var sv : [0..<unrollFactor] elementType;
  foreach i in 1..(n & ~(unrollFactor - 1)) by unrollFactor do
    sv += x[i..#unrollFactor] * y[i..#unrollFactor];

  var s: elementType;
  if unrollFactor == 8 {
    const st4 = sv[0..#4] + sv[4..#4];
    const st2 = st4[0..#2] + st4[2..#2];
    s = st2[0] + st2[1];
  } else if unrollFactor == 4 {
    const st2 = sv[0..#2] + sv[2..#2];
    s = st2[0] + st2[1];
  } else {
    compilerAssert(unrollFactor == 2);
    s = sv[0] + sv[1];
  }
  return s;
}
// An unrolled version of the dot product, using a param for loop
// (which is semanticlly unrolled before the backend gets involved)
proc dotProdParamFor(n: int, x: [1..n] elementType, y: x.type): elementType {
  var sv : [0..<unrollFactor] elementType;
  foreach i in 1..(n & ~(unrollFactor - 1)) by unrollFactor {
    for param j in 0..<unrollFactor {
      sv[j] += x[i+j] * y[i+j];
    }
  }

  var s: elementType;
  for param i in 0..<unrollFactor {
    s += sv[i];
  }
  return s;
}
// The same as `dotProdParamFor`, using a `c_array` to avoid some Chapel array overhead
proc dotProdParamForCArray(n: int, x: [1..n] elementType, y: x.type): elementType {
  var sv : c_array(elementType, unrollFactor);
  foreach i in 1..(n & ~(unrollFactor - 1)) by unrollFactor {
    for param j in 0..<unrollFactor {
      sv[j] += x[i+j] * y[i+j];
    }
  }

  var s: elementType;
  for param i in 0..<unrollFactor {
    s += sv[i];
  }
  return s;
}
// The same as `dotProdFor`, but using LLVM backend hints to unroll the loop
proc dotProdMetadataUnrollFor(n: int, x: [1..n] elementType, y: x.type): elementType {
  var s: elementType;
  // an unfortunate current limitation of llvm.metadata is that it only accepts literals
  if unrollFactor == 8 {
    @llvm.metadata(("llvm.loop.unroll.count", 8))
    for i in 1..n do s += x[i] * y[i];
  } else if unrollFactor == 4 {
    @llvm.metadata(("llvm.loop.unroll.count", 4))
    for i in 1..n do s += x[i] * y[i];
  } else {
    compilerAssert(unrollFactor == 2);
    @llvm.metadata(("llvm.loop.unroll.count", 2))
    for i in 1..n do s += x[i] * y[i];
  }
  return s;
}
// The same as `dotProdForeach`, but using LLVM backend hints to unroll the loop
proc dotProdMetadataUnrollForeach(n: int, x: [1..n] elementType, y: x.type): elementType {
  var s: elementType;
  // an unfortunate current limitation of llvm.metadata is that it only accepts literals
  if unrollFactor == 8 {
    @llvm.metadata(("llvm.loop.unroll.count", 8))
    foreach i in 1..n with (ref s) do s += x[i] * y[i];
  } else if unrollFactor == 4 {
    @llvm.metadata(("llvm.loop.unroll.count", 4))
    foreach i in 1..n with (ref s) do s += x[i] * y[i];
  } else {
    compilerAssert(unrollFactor == 2);
    @llvm.metadata(("llvm.loop.unroll.count", 2))
    foreach i in 1..n with (ref s) do s += x[i] * y[i];
  }
  return s;
}

proc main() {
  var x, y: [1..N] elementType;

  var rand = new randomStream(elementType, seed);
  rand.fill(x, minRand, maxRand);
  rand.fill(y, minRand, maxRand);

  var timer = new stopwatch();

  var s: elementType;
  timer.start();
  for 1..iterations do s += dotProdFor(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdFor", s);
    else writef("%30s time: %r\n", "dotProdFor", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdForeach(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdForeach", s);
    else writef("%30s time: %r\n", "dotProdForeach", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdChapeltastic(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdChapeltastic", s);
    else writef("%30s time: %r\n", "dotProdChapeltastic", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdSlices(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdSlices", s);
    else writef("%30s time: %r\n", "dotProdSlices", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdParamFor(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdParamFor", s);
    else writef("%30s time: %r\n", "dotProdParamFor", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdParamForCArray(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdParamForCArray", s);
    else writef("%30s time: %r\n", "dotProdParamForCArray", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdMetadataUnrollFor(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdMetadataUnrollFor", s);
    else writef("%30s time: %r\n", "dotProdMetadataUnrollFor", timer.elapsed());
  timer.reset();

  s = 0;
  timer.start();
  for 1..iterations do s += dotProdMetadataUnrollForeach(N, x, y);
  timer.stop();
  if correctness
    then writef("%30s: %?\n", "dotProdMetadataUnrollForeach", s);
    else writef("%30s time: %r\n", "dotProdMetadataUnrollForeach", timer.elapsed());
  timer.reset();

}
