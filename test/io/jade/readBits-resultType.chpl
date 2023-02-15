use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);

var w = f.writer(kind=ionative);
var b = [0xaa66, 0x7852, 0x9287, 0x1455, 0x2299, 0x7218, 0x8278, 0x4082] :uint(16);
w.writeBinary(b);
w.close();

var r = f.reader(kind=ionative);

var t1 = r.readBits(uint(8), 8);
writeln("Read value ", t1);
assert(t1 == 0x66);

var t2 = r.readBits(int(16), 16);
writeln("Read value ", t2);
assert(t2 == 0xaa52:int(16));

var t3 = r.readBits(int, 3);
writeln("Read value ", t3);
assert(t3 == 0b011);

r.close();
f.close();

FileSystem.remove(testfile);
