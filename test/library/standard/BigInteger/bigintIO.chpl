
use BigInteger;
use IO;
use FileSystem;

config var filename = "test.txt";
var f = open(filename, ioMode.cwr);

{
  var w = f.writer(locking=false);

  var a = 17:bigint;
  w.writeln(a);
  a = 19:bigint;
  w.writeln(a);
}

{
  var r = f.reader(locking=false);

  var a: bigint;
  r.read(a);
  writeln("read ", a);

  r.read(a);
  writeln("read ", a);
}


remove(filename);
