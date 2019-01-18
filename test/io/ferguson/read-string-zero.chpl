
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
  var got = r.readstring(s, str.length);

  assert(got);
  writeln("readstring read a string with length ", s.length);
  assert(s.length == str.length);
  assert(s == str);
}

{
  // test 2: readf
  var r = f.reader(kind=iokind.native);

  var s:string;
  var len = str.length;
  var got = r.readf("%|*s", len, s);

  assert(got);
  writeln("readf read a string with length ", s.length);
  assert(s.length == str.length);
  assert(s == str);
}

f.close();
