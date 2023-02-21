use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);
var r = f.writer(kind=ionative);
var tmp = 0b101;

// should throw deprecation
r.writebits(tmp, 3);

FileSystem.remove(testfile);

