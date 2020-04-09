use IO except openmem; // to test deprecation warning for openmem
// was originally test/io/ferguson/readThis/readclass.chpl
class mything {
  var x:int;

  proc readWriteThis(rw) throws {
    rw <~> x;
  }
}

class subthing : mything {
  var y:int;

  override proc readWriteThis(rw) throws {
    rw <~> x;
    rw <~> new ioLiteral(",");
    rw <~> y;
  }
}


{
  var a = new borrowed mything(1);

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = new borrowed mything(2);

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
}

{
  var a = new borrowed subthing(3,4);

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = new borrowed subthing(5,6);

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
  assert(a.y == b.y);
}

{
  var a = new borrowed subthing(3,4);

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = new borrowed subthing(5,6);
  var c:borrowed mything = b;

  r.read(c);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
  assert(a.y == b.y);
}


