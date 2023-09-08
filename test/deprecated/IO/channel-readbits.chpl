use FileSystem;
use IO;

config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);
var r = f.reader(deserializer=new binaryDeserializer());
var tmp:uint(64);

// should throw deprecation
r.readbits(tmp, 3);

FileSystem.remove(testfile);
