use Sort;
use Random;
use Time;

config const printStats = true;
config const minSize = 1;
config const maxSize = 1024*1024*128;

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
  fillRandom(array);
 

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
    //if printStats then writef(" % 10s", t.elapsed());
    if printStats then writef(" % 12s", mibs:string);
    assert(isSorted(input));
  }
  writeln();
}

proc main() {
  writef("% 16s", "size");

  for m in methods {
    writef(" % 12s", m);
  }
  writeln();

  var size = minSize;
  while size <= maxSize {
    testsize(size);
    size *= 2;
  }
}
