use IO;

config const output = "output.bin";
var f = open(output, iomode.cwr);
var w = f.writer(kind=iokind.native, locking=false);
for i in 0..255 {
  var byte:uint(8) = i:uint(8);
  w.write(byte);
}
