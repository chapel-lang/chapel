import Sort;
use IO;


proc testText(ref seeds: domain(int)) {
  writeln("in testText ", Sort.sorted(seeds));
  var lf = open('test.log' : string, ioMode.cwr);
  var delta: [seeds] real;
  var po: domain(int);
  var pk: [po] real;
  seeds.add(1);
  delta[1] = 12;
  var c = lf.writer(locking=false);
  var z = lf.reader(locking=false);
  c.write(seeds);
  c.flush();
  writeln("Wrote: ", Sort.sorted(seeds));
  var ok = z.read(po);
  assert(ok);
  writeln("Read: ", Sort.sorted(po));
}
proc testBinary(ref seeds: domain(int)) {
  writeln("in testBinary ", Sort.sorted(seeds));
  var lf = open('test.log' : string, ioMode.cwr);
  var delta: [seeds] real;
  var po: domain(int);
  var pk: [po] real;
  seeds.add(1);
  delta[1] = 12;
  var c = lf.writer(serializer=new binarySerializer(), locking=false);
  var z = lf.reader(deserializer=new binaryDeserializer(), locking=false);
  c.write(seeds);
  c.flush();
  writeln("Wrote: ", Sort.sorted(seeds));
  var ok = z.read(po);
  assert(ok);
  writeln("Read: ", Sort.sorted(po));
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
