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

var methods = ["default", "msbRadixSort", "quickSort"];

proc testsort(input, method, parallel) {
  if method == "default" {
    if parallel == false {
      serial { sort(input); }
    } else {
      sort(input);
    }
  } else if method == "msbRadixSort" {
    if parallel == false {
      serial { msbRadixSort(input, defaultComparator); }
    } else {
      msbRadixSort(input, defaultComparator);
    }
  } else if method == "quickSort" {
    if parallel == false {
      serial { quickSort(input); }
    } else {
      quickSort(input);
    }
  }
}

proc testsize(size:int) {
  var array:[1..size] int; 

  if inputDataScheme == 0 {
    // scheme 0 : all zeros
  } else if inputDataScheme == 1 {
    // scheme 1: random ints
    fillRandom(array);
  } else if inputDataScheme == 2 {
    // scheme 2: random ints, only top byte set
    fillRandom(array);
    for a in array {
      a >>= 56;
      a <<= 56;
    }
  } else if inputDataScheme == 3 {
    // scheme 3: random ints, only a middle byte set
    fillRandom(array);
    for a in array {
      a >>= 56;
      a <<= 56;
      a >>= 16;
    }
  } else if inputDataScheme == 4 {
    // scheme 4: random ints, only bottom byte set
    fillRandom(array);
    for a in array {
      a &= 0xff;
    }
  }

  var bytes = size*8;
  var kibibytes = bytes/1024.0;
  var mibibytes = kibibytes/1024.0;
  var sizestr = bytes + " bytes";
  if kibibytes >= 1.0 then
    sizestr = kibibytes + " KiB";
  if mibibytes >= 1.0 then
    sizestr = mibibytes + " MiB";
  if printStats then
    writef("% 16s", sizestr);

  var input = array;

  var ntrials = 1;
  if mibibytes < 1 then
    ntrials = 10;
  if kibibytes < 1 then
    ntrials = 100;

  for m in methods {
    var t: Timer;
    for i in 1..ntrials {
      input = array;
      t.start();
      testsort(input, m, parallel);
      t.stop();
    }
    var mibs = mibibytes * ntrials / t.elapsed();
    if printStats then writef(" % 12s", mibs:string);
    assert(isSorted(input));
  }
  if printStats then writeln();
}

proc main() {
  if printStats then
    writef("% 16s", "size");

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
