use IO;

class mything {
  var x:int;
  var y:int;

  proc readThis(r) throws {
    x = r.read(int);
    r.readLiteral(" ");
    y = r.read(int);
  }

  proc writeThis(w) throws {
    w.write(x);
    w.writeLiteral(" ");
    w.write(y);
  }

}

{
  var a = (new owned mything(1)).borrow();

  writeln("Writing ", a);

  var f = openmem();
  var w = f.writer();

  w.write(a);
  w.close();

  var r = f.reader();

  var b = (new owned mything(2)).borrow();

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
}

