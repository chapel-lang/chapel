use IO;

var f = openmem();

{
  f.writer().write("+10");
}

{
  var x:int;
  f.reader().read(x);
  writeln(x);
}

{
  f.writer().write("+100.0");
}

{
  var x:real;
  f.reader().read(x);
  writeln(x);
}

{
  f.writer().write("+10000.0i");
}

{
  var x:imag;
  f.reader().read(x);
  writeln(x);
}
