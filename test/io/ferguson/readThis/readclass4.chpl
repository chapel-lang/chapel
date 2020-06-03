use IO;

class mything {
  var x:int;
  var y:int;
  proc writeThis(w: Writer) throws {
    w & x;
    w & new ioLiteral(" ");
    w & y;
  }
  // no readThis. Expect a compile-time error
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
}

