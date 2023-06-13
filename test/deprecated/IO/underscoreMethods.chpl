// deprecated by jade in 1.31
use IO;
config var filename = "test.txt";

var f = open(filename, ioMode.cwr);

{
  writeln("writing");
  var w = f.writer(locking=true);
  w.lock();

  w._mark();
  w.writeln("hello world");
  w._revert();

  var off = w._offset();
  writeln(off);

  w._mark();
  w.writeln("world hello");
  w._commit();

  off = w._offset();
  writeln(off);

  w.unlock();
}
// use to put a space in .good to make more readable
compilerWarning("");
{
  writeln("reading");
  var r = f.reader(locking=true);
  r.lock();

  r._mark();
  var line = r.readLine(stripNewline=true);
  if line != "hello world" then r._revert();

  var off = r._offset();
  writeln(off);

  r._mark();
  line = r.readLine(stripNewline=true);
  if line == "world hello" then r._commit();

  off = r._offset();
  writeln(off);

  r.unlock();
}
