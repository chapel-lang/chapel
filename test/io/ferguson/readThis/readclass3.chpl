class mything {
  var x:int;
  var y:int;
  proc readThis(r) {
    r <~> x;
    r <~> new ioLiteral(" ");
    r <~> y;
  }
  proc writeThis(w) {
    w <~> x;
    w <~> new ioLiteral(" ");
    w <~> y;
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
}

