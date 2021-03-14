
use IO;

var str = "hello\x00goodbye\n";

var f = opentmp();

{
  var w = f.writer(kind=iokind.native);
  w.write(str);
  w.close();
}

{
  // test 1: readstring
  var r = f.reader(kind=iokind.native);

  var s:string;
  var got = r.readstring(s, str.numBytes);

  assert(got);
  writeln("readstring read a string with length ", s.numBytes);
  assert(s.numBytes == str.numBytes);
  assert(s == str);
}

{
  // test 2: readf
  var r = f.reader(kind=iokind.native);

  var s:string;
  var len = str.numBytes;
  var got = r.readf("%|*s", len, s);

  assert(got);
  writeln("readf read a string with length ", s.numBytes);
  assert(s.numBytes == str.numBytes);
  assert(s == str);
}

f.close();
