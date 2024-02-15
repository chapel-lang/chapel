use IO;

config const output = "output.bin";
var w = openWriter(output);
for i in 0..255 {
  var byte:uint(8) = i:uint(8);
  w.writeBinary(byte);
}
