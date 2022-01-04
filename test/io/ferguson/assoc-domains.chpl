use IO;


proc testText(ref seeds: domain(int)) {
  writeln("in testText ", seeds.sorted());
  var lf = open('test.log' : string, iomode.cwr);
  var delta: [seeds] real;
  var po: domain(int);
  var pk: [po] real;
  seeds.add(1);
  delta[1] = 12;
  var c = lf.writer();
  var z = lf.reader();
  c.write(seeds);
  c.flush();
  writeln("Wrote: ", seeds.sorted());
  var ok = z.read(po);
  assert(ok);
  writeln("Read: ", po.sorted());
}
proc testBinary(ref seeds: domain(int)) {
  writeln("in testBinary ", seeds.sorted());
  var lf = open('test.log' : string, iomode.cwr);
  var delta: [seeds] real;
  var po: domain(int);
  var pk: [po] real;
  seeds.add(1);
  delta[1] = 12;
  var c = lf.writer(kind=ionative);
  var z = lf.reader(kind=ionative);
  c.write(seeds);
  c.flush();
  writeln("Wrote: ", seeds.sorted());
  var ok = z.read(po);
  assert(ok);
  writeln("Read: ", po.sorted());
}

proc test(in seeds) {
  testText(seeds);
  testBinary(seeds);
}

var emptyDomain:domain(int);
test(emptyDomain);
test({1});
test({1,2});
test({1,2,3});
