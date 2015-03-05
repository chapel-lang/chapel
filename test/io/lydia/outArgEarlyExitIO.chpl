use IO;

// This function returns its result directly and
// does not segfault.
proc normalReturn(doWhat: bool): bool {
  if (doWhat) {
    return false;
  }

  // This code won't get executed in the calls made by
  // this test.
  var f1 = open("filename.txt", iomode.r);
  var f2 = open("file2.txt", iomode.cw);
  var c1 = f1.reader();
  var c2 = f2.writer();
  var line: string;
  while (c1.readline(line)) {
    c2.write(line);
  }
  c2.flush();
  f1.close();
  f2.close();
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
  var f1 = open("filename.txt", iomode.r);
  var f2 = open("file2.txt", iomode.cw);
  var c1 = f1.reader();
  var c2 = f2.writer();
  var line: string;
  while (c1.readline(line)) {
    c2.write(line);
  }
  c2.flush();
  f1.close();
  f2.close();
  res = true;
}

var res1: bool = normalReturn(true);
writeln(res1); // Should output false

var res2: bool;
outReturn(res2, true);
// At the time this test was filed, the previous command
// would segfault.
writeln(res2); // Should also output false.

