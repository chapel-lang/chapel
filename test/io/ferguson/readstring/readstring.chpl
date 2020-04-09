use IO;

config const filename = "test.txt";

var f = open("test.txt", iomode.r);

{
  var r = f.reader(kind=ionative);
  var str:string;
  r.readstring(str);
  writeln("Read (binary all)\n", str);
  r.close();
}

{
  var r = f.reader(kind=ionative);
  var str:string;
  r.readstring(str, 10);
  writeln("Read (binary 10)\n", str);
  r.close();
}

{
  var r = f.reader();
  var str:string;
  r.readstring(str);
  writeln("Read (text all)\n", str);
  r.close();
}

{
  var r = f.reader();
  var str:string;
  r.readstring(str, 10);
  writeln("Read (text 10)\n", str);
  r.close();
}


f.close();
