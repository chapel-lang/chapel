use IO;

var f = openMemFile();

{
  f.writer(locking=false).write("+10");
}

{
  var x:int;
  f.reader(locking=false).read(x);
  writeln(x);
}

{
  f.writer(locking=false).write("+100.0");
}

{
  var x:real;
  f.reader(locking=false).read(x);
  writeln(x);
}

{
  f.writer(locking=false).write("+10000.0i");
}

{
  var x:imag;
  f.reader(locking=false).read(x);
  writeln(x);
}
