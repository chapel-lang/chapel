use IO;

config var filename = "test.txt";

{
  var f = open(filename, ioMode.cw);
  var w = f.writer(locking=false);
  w.writeln("hello world from writeln");
  w.writeln("hello world from writeln again");
  w.close();
  f.close();
}

{
  writeln(ioHintSet.mmap(false)._internal);
  // works with ioHintSet.fromFlag(QIO_METHOD_FREADFWRITE)
  // ioHintSet.fromFlag(QIO_METHOD_FREADFWRITE)
  var f = open(filename, ioMode.a); // open then close without a write is fine
  var a = f.writer(); //region=f.size..
  a.writeln("world hello");
  a.close();
  f.close();
}

{
  var r = openReader(filename);
  var line: string;
  while r.readLine(line) {
    write(line);
  }
  r.close();
}
