use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);
var r = f.writer(kind=ionative);


// should throw deprecation
r.writebits(0x41, 8);
r.writeBits(0x41, 8);
r.writeBits(0xA, 8);
