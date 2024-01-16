
use BigInteger;
use IO;
use FileSystem;

config var filename = "test.txt";
var f = open(filename, ioMode.cwr);

{
  var w = f.writer();

  var a = 17:bigint;
  w.writeln(a);
  a = 19:bigint;
  w.writeln(a);
}

{
  var r = f.reader();

  var a: bigint;
  r.read(a);
  writeln("read ", a);

  r.read(a);
  writeln("read ", a);
}


remove(filename);
