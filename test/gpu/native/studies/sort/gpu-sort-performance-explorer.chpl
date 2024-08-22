// Adapted from test/library/packages/Sort/performance/sort-performance-explorer.chpl
// This tester allows exploration of sort performance
// for differing algorithms, input data distributions, and input sizes.

use Sort;
use Random;
use Time;
use CTypes;
import GPU;

config const printStats = true;
config const minSize = 1<<24;
config const maxSize = 1<<27;
config const chunkSize = 6250;
config const bitsAtATime = 8;

config const inputDataScheme = 1;
config type eltType = uint(32);

config const seed = (new randomStream(int(32))).seed;

var methods = ["default", "gpuCub", "gpuRadix"];

proc testsort(ref gpuArr, method) {

  if method == "gpuCub" {
      GPU.gpuExternSort(gpuArr);
  } else if method == "gpuRadix" {
    GPU.parallelRadixSort(gpuArr, bitsAtATime, chunkSize);
  } else if method == "default" {
    GPU.gpuSort(gpuArr);
  } else {
    halt("Unknown sorting method " + method);
  }
}

proc makeInput(array) {
  if isRealType(eltType) {
    var result = forall a in array do (a/1000.0 + a - 500.0):eltType;
    return result;
  } else {
    var result = forall a in array do a:eltType;
    return result;
  }
}

proc generateArray(size:int, describe=false) {
  var array:[1..size] int;

  if inputDataScheme == 0 {
    // scheme 0 : all zeros
    if describe then
      writeln("generating all zeros input");
  } else if inputDataScheme == 1 {
    // scheme 1: random ints
    if describe then
      writeln("generating random input");
    fillRandom(array, seed=seed);
  } else if inputDataScheme == 2 {
    // scheme 2: random ints, only top byte set
    if describe then
      writeln("generating random input setting only the top byte");
    fillRandom(array, seed=seed);
    for a in array {
      a >>= 56;
      a <<= 56;
    }
  } else if inputDataScheme == 3 {
    // scheme 3: random ints, only a middle byte set
    if describe then
      writeln("generating random input setting only a middle byte");
    fillRandom(array, seed=seed);
    for a in array {
      a >>= 56;
      a <<= 56;
      a >>= 16;
    }
  } else if inputDataScheme == 4 {
    // scheme 4: random ints, only bottom byte set
    if describe then
      writeln("generating random input setting only the bottom byte");
    fillRandom(array, seed=seed);
    for a in array {
      a &= 0xff;
    }
  } else if inputDataScheme == 5 {
    // scheme 5: heavily skewed distribution,
    // values are (1 << (random % 64))
    if describe then
      writeln("generating random powers of 2 input");
    fillRandom(array, seed=seed);
    for a in array {
      var shift = mod(a, 64);
      a = 1 << shift;
    }
  } else if inputDataScheme == 6 {
    // scheme 6: data is already in sorted order
    if describe then
      writeln("generating already-sorted input");
    array = 1..size;
  } else if inputDataScheme == 7 {
    // scheme 7: data is in reverse sorted order
    if describe then
      writeln("generating reverse-sorted input");
    array = 1..size by -1;
  }

  return array;
}

proc testsize(size:int) {

  var array = generateArray(size);

  var nBytes = size*numBytes(eltType);
  var kibibytes = nBytes/1024.0;
  var mibibytes = kibibytes/1024.0;
  var sizestr = nBytes:string + " bytes";
  if kibibytes >= 1.0 then
    sizestr = kibibytes:string + " KiB";
  if mibibytes >= 1.0 then
    sizestr = mibibytes:string + " MiB";
  if printStats then
    writef("% 16s", sizestr);

  var input = makeInput(array);

  var ntrials = 1;
  if printStats {
    // use more trials for small problem sizes when measuring speed
    if mibibytes < 1 then
      ntrials = 100;
    if kibibytes < 100 then
      ntrials = 1_000;
    if kibibytes < 0.4 then
      ntrials = 10_000;
  }

  for m in methods {
    var t: stopwatch;
    for i in 1..ntrials {
      input = makeInput(array);
      on here.gpus[0]{
        var gpuArr = input; // Copy to gpu
        t.start();
        testsort(gpuArr, m);
        t.stop();
        input = gpuArr; // Copy back to cpu
      }
    }
    var mibs = mibibytes * ntrials / t.elapsed();
    if printStats then writef(" % 12s sec  ", t.elapsed():string);
    if printStats then writef(" % 9s MiB/s", mibs:string);

    assert(isSorted(input));
  }
  if printStats then writeln();
}

proc main() {
  // run generateArray to output the distribution
  generateArray(100, describe=printStats);

  if printStats {
    writeln("Note, speeds are in MiB/s");
    writef("% 16s", "size");
  }

  for m in methods {
    if printStats then
      writef(" % 30s", m);
  }
  if printStats then
    writeln();

  var size = minSize;
  while size <= maxSize {
    testsize(size);
    size *= 2;
  }
}
