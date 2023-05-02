// deprecated by jade in 1.31
use IO;
config var filename = "test.txt";
config param useLocking = true;

var f = open(filename, ioMode.cwr);

{
  writeln("writing");
  var w = f.writer(locking=useLocking);
  var off = w.offset();
  writeln(off);
  w.writeln("hello world");
  off = w.offset();
  writeln(off);
}

{
  writeln("reading");
  var r = f.reader(locking=useLocking);
  var off = r.offset();
  writeln(off);
  var line = r.readLine(stripNewline=true);
  assert(line == "hello world");
  off = r.offset();
  writeln(off);
}
