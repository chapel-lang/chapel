use IO;

var f = open("test.txt", ioMode.r);

{
  var r = f.reader(kind=ionative);
  var str:bytes;
  r.readBytes(str, 21);
  writeln("Read (binary 21)\n", str);
  r.close();
}

{
  var r = f.reader();
  var str:bytes;
  r.readBytes(str, 21);
  writeln("Read (text 21)\n", str);
  r.close();
}

f.close();
