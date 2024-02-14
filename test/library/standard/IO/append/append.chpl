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
  var f = open(filename, ioMode.a);
  var a = f.writer(locking=false);
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
