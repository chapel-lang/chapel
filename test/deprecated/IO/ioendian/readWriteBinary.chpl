use IO;

var f = openMemFile();


proc writeout(arg:numeric) {
    f.writer(locking=false).writeBinary(arg, ioendian.little);
    f.writer(locking=false).writeBinary(arg, ioendian.big);
    f.writer(locking=false).writeBinary(arg, ioendian.native);
}

proc readin(ref arg:int) {
  f.reader(locking=false).readBinary(arg, ioendian.little);
  f.reader(locking=false).readBinary(arg, ioendian.big);
  f.reader(locking=false).readBinary(arg, ioendian.native);

}


writeout(0x15678);
writeout(0x12345678);
writeout(0x31);
var x : int;
readin(x);
readin(x);
readin(x);
