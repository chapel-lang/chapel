// This program sometimes fails on two or more locales, either getting
// an out-of-bounds error or a segfault.  For a single locale, there
// is no error.  If the strings are changed to integers, it doesn't
// seem to get an error either, suggesting that something about using
// strings across multiple locales may be the issue.  Running with a
// smaller number of entries doesn't seem to solve the problem either
// (I wondered for awhile whether the array resizing was causing
// problems, but see failures with 3 entries where no resizing should
// be done).
//
use IO;

config const ENTRIES = 15,
             debug = false;

var keys: [0..#ENTRIES] string;
for i in 0..#ENTRIES {
  keys[i] = "%04i".format(i);
  writeln(keys);
}

on Locales [numLocales-1] {
  var D: domain(string);
  var array: [D] uint(64);

  if debug then
    writeln("D is ", D.locale, ", array is ", array.locale);
		
  for i in 0..#ENTRIES {
    writeln("assign array [\"%s\"] = 12345".format(keys[i]));
    D += keys[i];
    array[keys[i]] = 12345;
  }

  if debug {
    writeln(D);
    for a in array do
      writeln(a);
    writeln(array);
  }
                
  for i in 0..#ENTRIES {
    writeln("get array[\"%s\"]: %i".format(keys[i], array[keys[i]]));
  }
}
