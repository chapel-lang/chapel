use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);

var w = f.writer(kind=ionative);
var b = [0xaa, 0x86, 0x78, 0x52, 0x92, 0x87, 0x14, 0x55, 0x22, 0x99, 0x72, 0x18, 0x82, 0x78, 0x40, 0x82] :uint(8);
w.writeBinary(b);
w.close();

var r = f.reader(kind=ionative);

var t1 = r.readBits(uint(8), 8);
writeln("Read value ", t1);
assert(t1 == 0xaa);

var t2 = r.readBits(int(16), 16);
writeln("Read value ", t2);
assert(t2 == 0x8678:int(16));

var t3 = r.readBits(int, 3);
writeln("Read value ", t3);
assert(t3 == 0b010);

r.close();
f.close();

// FileSystem.remove(testfile);
