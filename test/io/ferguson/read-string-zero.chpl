
use IO;

var str = "hello\x00goodbye\n";

var f = openTempFile();

{
  var w = f.writer(locking=false);
  w.writeString(str);
  w.close();
}

{
  // test 1: readstring
  var r = f.reader(locking=false);

  var s:string;
  var got = r.readString(s, str.numBytes);

  assert(got);
  writeln("readstring read a string with length ", s.numBytes);
  assert(s.numBytes == str.numBytes);
  assert(s == str);
}

f.close();
