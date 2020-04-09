use IO;

class R {
  var a:int;
  var b:int;
  var c:int;
  var d:int;
  var e:int;
  proc equals(x:borrowed R) {
    return a == x.a && b == x.b && c == x.c && d == x.d && e == x.e;
  }
}

var f = open("binary-output.bin", iomode.cwr);

var A = new borrowed R(1,2,3,4,5);

{
  var w = f.writer(kind=iobig);
  writeln("Writing ", A);
  w.write(A);
  w.close();
}

{
  var r = f.reader(kind=iobig);
  var B = new borrowed R(0,0,0,0,0);

  r.read(B);
  writeln("Read ", B);

  assert(B.equals(A));
}


{
  var r = f.reader(kind=iobig);
  var B = new borrowed R(0,0,0,0,0);

  assert(r.read(B.a));
  assert(r.read(B.b));
  assert(r.read(B.c));
  assert(r.read(B.d));
  assert(r.read(B.e));

  writeln("Read again ", B);

  assert(B.equals(A));
}


f.close();
