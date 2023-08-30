use IO;

var filename = "remote-file.chpl";
var f = open(filename, ioMode.r);

for loc in Locales do on loc {
  var firstval: int(8);
  var ch = f.reader(deserializer=new BinaryDeserializer());
  ch.read(firstval);
  writeln(loc, " read ", firstval);
}
