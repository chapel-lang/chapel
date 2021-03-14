use IO;

class mything {
  var x:int;
  var y:int;

  proc readThis(r) throws {
    r <~> x;
    r <~> new ioLiteral(" ");
    r <~> y;
  }

  proc writeThis(w) throws {
    w <~> x;
    w <~> new ioLiteral(" ");
    w <~> y;
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

