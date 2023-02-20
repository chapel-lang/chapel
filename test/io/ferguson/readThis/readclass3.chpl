use IO;

class mything {
  var x:int;
  var y:int;
  proc init(x: int = 0, y: int = 0) {
    this.x = x;
    this.y = y;
  }
  proc init(r: fileReader) {
    this.x = r.read(int);
    r.readLiteral(" ");
    this.y = r.read(int);
  }

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

  var f = openMemFile();
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

