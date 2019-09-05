// This tester allows exploration of sort performance
// for differing algorithms, input data distributions, and input sizes.

use Sort;
use Random;
use Time;

config const printStats = true;
config const minSize = 1;
config const maxSize = 1024*1024*128;

config const inputDataScheme = 1;

config const parallel = true;

config param reverse = false;
config type eltType = int;

config const seed = SeedGenerator.oddCurrentTime;

var methods = ["default", "msbRadixSort", "quickSort", "mergeSort",
               "twoArraySample", "twoArrayRadix"];

proc testsort(input, method, parallel, cmp) {

  if method == "default" {
    if parallel == false {
      serial { sort(input, cmp); }
    } else {
      sort(input, cmp);
    }
  } else if method == "msbRadixSort" {
    if parallel == false {
      serial { MSBRadixSort.msbRadixSort(input, cmp); }
    } else {
      MSBRadixSort.msbRadixSort(input, cmp);
    }
  } else if method == "quickSort" {
    if parallel == false {
      serial { QuickSort.quickSort(input, comparator=cmp); }
    } else {
      QuickSort.quickSort(input, comparator=cmp);
    }
  } else if method == "mergeSort" {
    if parallel == false {
      serial { MergeSort.mergeSort(input); }
    } else {
      MergeSort.mergeSort(input);
    }
  } else if method == "twoArraySample" {
    if parallel == false {
      serial { TwoArraySampleSort.twoArraySampleSort(input); }
    } else {
      TwoArraySampleSort.twoArraySampleSort(input);
    }
  } else if method == "twoArrayRadix" {
    if parallel == false {
      serial { TwoArrayRadixSort.twoArrayRadixSort(input); }
    } else {
      TwoArrayRadixSort.twoArrayRadixSort(input);
    }
  } else {
    halt("Unknown sorting method " + method);
  }
}

proc doString() param {
  return eltType == string || eltType == c_string;
}
proc makeInput(array, inputStrings) {
  if doString() {
    if eltType == c_string {
      var result = forall a in inputStrings do a.c_str();
      return result;
    } else {
      return inputStrings;
    }
  } else {
    if isFloatType(eltType) {
      var result = forall a in array do (a/1000.0 + a - 500.0):eltType;
      return result;
    } else {
      var result = forall a in array do a:eltType;
      return result;
    }
  }
}

proc testsize(size:int) {
  var array:[1..size] int;

  if inputDataScheme == 0 {
    // scheme 0 : all zeros
  } else if inputDataScheme == 1 {
    // scheme 1: random ints
    fillRandom(array, seed=seed);
  } else if inputDataScheme == 2 {
    // scheme 2: random ints, only top byte set
    fillRandom(array, seed=seed);
    for a in array {
      a >>= 56;
      a <<= 56;
    }
  } else if inputDataScheme == 3 {
    // scheme 3: random ints, only a middle byte set
    fillRandom(array, seed=seed);
    for a in array {
      a >>= 56;
      a <<= 56;
      a >>= 16;
    }
  } else if inputDataScheme == 4 {
    // scheme 4: random ints, only bottom byte set
    fillRandom(array, seed=seed);
    for a in array {
      a &= 0xff;
    }
  } else if inputDataScheme == 5 {
    // scheme 5: heavily skewed distribution,
    // values are (1 << (random % 64))
    fillRandom(array, seed=seed);
    for a in array {
      var shift = mod(a, 64);
      a = 1 << shift;
    }
  }


  var inputStringsDomain = {1..0};
  var inputStrings:[inputStringsDomain] string;
  if doString() {
    inputStringsDomain = array.domain;
    inputStrings = forall a in array do a:string;
  }

  var nBytes = size*8;
  var kibibytes = nBytes/1024.0;
  var mibibytes = kibibytes/1024.0;
  var sizestr = nBytes:string + " bytes";
  if kibibytes >= 1.0 then
    sizestr = kibibytes:string + " KiB";
  if mibibytes >= 1.0 then
    sizestr = mibibytes:string + " MiB";
  if printStats then
    writef("% 16s", sizestr);

  var input = makeInput(array, inputStrings);

  var ntrials = 1;
  if mibibytes < 1 then
    ntrials = 10;
  if kibibytes < 1 then
    ntrials = 100;

  for m in methods {
    const ref cmp = if reverse then reverseComparator else defaultComparator;
    var t: Timer;
    for i in 1..ntrials {
      input = makeInput(array, inputStrings);
      t.start();
      testsort(input, m, parallel, cmp);
      t.stop();
    }
    var mibs = mibibytes * ntrials / t.elapsed();
    if printStats then writef(" % 12s", mibs:string);
    assert(isSorted(input, cmp));
  }
  if printStats then writeln();
}

proc main() {
  if printStats {
    writeln("Note, speeds are in MiB/s");
    writef("% 16s", "size");
  }

  for m in methods {
    if printStats then
      writef(" % 12s", m);
  }
  if printStats then
    writeln();

  var size = minSize;
  while size <= maxSize {
    testsize(size);
    size *= 2;
  }
}
