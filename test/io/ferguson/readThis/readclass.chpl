class mything {
  var x:int;

  proc readWriteThis(rw) {
    rw <~> x;
  }
}

class subthing : mything {
  var y:int;

  proc readWriteThis(rw) {
    rw <~> x;
    rw <~> new ioLiteral(",");
    rw <~> y;
  }
}


{
  var a = new mything(1);

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = new mything(2);

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);

  delete b;
  delete a;
}

{
  var a = new subthing(3,4);

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = new subthing(5,6);

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
  assert(a.y == b.y);

  delete b;
  delete a;
}

{
  var a = new subthing(3,4);

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = new subthing(5,6);
  var c:mything = b;

  r.read(c);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
  assert(a.y == b.y);

  delete b;
  delete a;
}


