use IO;

// This function returns its result directly and
// does not segfault.
proc normalReturn(doWhat: bool): bool {
  if (doWhat) {
    return false;
  }

  // This code won't get executed in the calls made by
  // this test.
  var f = open("filename.txt", iomode.r);
  var c = f.reader();
  var line: string;
  while (c.readline(line)) {
    write(line);
  }
  f.close();
  return true;
}

// This function segfaults in the early exit case,
// due to attempting to close the channel
proc outReturn(out res: bool, doWhat: bool) {
  if (doWhat) {
    res = false;
    return;
  }

  // This code won't get executed in the calls made by
  // this test.
  var f = open("filename.txt", iomode.r);
  var c = f.reader();
  var line: string;
  while (c.readline(line)) {
    write(line);
  }
  f.close();
  res = true;
}

var res1: bool = normalReturn(true);
writeln(res1); // Should output false

var res2: bool;
outReturn(res2, true);
// At the time this test was filed, the previous command
// would segfault.
writeln(res2); // Should also output false.

