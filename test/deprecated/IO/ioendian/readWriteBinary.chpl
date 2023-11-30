use IO;

var f = openMemFile();


proc writeout(arg:numeric) {
    f.writer().writeBinary(arg, ioendian.little);
    f.writer().writeBinary(arg, ioendian.big);
    f.writer().writeBinary(arg, ioendian.native);
}

proc readin(ref arg:int) {
  f.reader().readBinary(arg, ioendian.little);
  f.reader().readBinary(arg, ioendian.big);
  f.reader().readBinary(arg, ioendian.native);

}


writeout(0x15678);
writeout(0x12345678);
writeout(0x31);
var x : int;
readin(x);
readin(x);
readin(x);
