use IO;

config const output = "output.bin";
var f = open(output, iomode.cwr);
var w = f.writer();
for i in 0..255 {
  var byte:uint(8) = i:uint(8);
  w.writef("%|1u", byte);
}
