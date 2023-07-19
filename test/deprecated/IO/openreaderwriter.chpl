use IO;
use FileSystem;

config var testfile = "test.txt";

{
  var w = openwriter(testfile);
  w.writeln("writing text");
}
{
  var r = openreader(testfile);
  var got = r.readLine(stripNewline=true);
  writeln(got);
}

FileSystem.remove(testfile);
