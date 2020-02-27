use IO;

record R {
  var a:int;
  var b:int;
  var c:int;
  var d:int;
  var e:int;
}

var f = open("binary-output.bin", iomode.cwr);

var A = new R(1,2,3,4,5);
{
  var w = f.writer(kind=iobig);
  writeln("Writing ", A);
  w.write(A);
  w.close();
}

{
  var r = f.reader(kind=iobig);
  var B = new R(0,0,0,0,0);
  r.read(B);
  writeln("Read ", B);
  assert(B==A);
}


{
  var r = f.reader(kind=iobig);
  var B = new R(0,0,0,0,0);
  assert(r.read(B.a));
  assert(r.read(B.b));
  assert(r.read(B.c));
  assert(r.read(B.d));
  assert(r.read(B.e));
  writeln("Read again ", B);
  assert(B==A);
}


f.close();
