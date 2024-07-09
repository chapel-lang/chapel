use IO;

config var filename = "test.txt";

{
  var f = open(filename, ioMode.a);
  var w = f.writer(locking=false);
  w.writeln("hello world from writeln");
  w.writeln("hello world from writeln again");
  w.close();
  f.close();
}
