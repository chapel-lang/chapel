use IO;

class mything {
  var x:int;
  var y:int;

  proc readWriteThis(rw) throws {
    rw <~> x;
    rw <~> new ioLiteral(" ");
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

