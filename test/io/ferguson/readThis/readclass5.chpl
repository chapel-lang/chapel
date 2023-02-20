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
    r.readNewline();
  }

  proc readThis(r) throws {
    readWriteHelper(r);
  }

  proc writeThis(w) throws {
    readWriteHelper(w);
  }

  proc readWriteHelper(rw) throws {
    if rw.writing then rw.write(x); else x = rw.read(int);
    rw.readWriteLiteral(" ");
    if rw.writing then rw.write(y); else y = rw.read(int);
    rw.readWriteNewline();
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

