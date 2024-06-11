use IO;

class mything : serializable {
  var x:int;
  proc init(x: int = 0) { this.x = x; }
  proc init(reader: fileReader(?), ref deserializer) {
    this.x = reader.read(int);
  }

  override proc deserialize(reader, ref deserializer) throws {
    reader.read(x);
  }

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }
}

class subthing : mything, serializable {
  var y:int;
  proc init(x: int = 0, y: int = 0) {
    super.init(x);
    this.y = y;
  }
  proc init(reader: fileReader(?), ref deserializer) {
    this.x = reader.read(int);
    reader.readLiteral(",");
    this.y = reader.read(int);
  }

  override proc deserialize(reader, ref deserializer) throws {
    x = reader.read(int);
    reader.readLiteral(",");
    y = reader.read(int);
  }

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
    writer.writeLiteral(",");
    writer.write(y);
  }
}


{
  var aOwn = new owned mything(1);
  var a = aOwn.borrow();

  writeln("Writing ", a);

  var f = openMemFile();
  var w = f.writer(locking=false);

  w.write(a);
  w.close();

  var r = f.reader(locking=false);

  var bOwn = new owned mything(2);
  var b = bOwn.borrow();

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
}

{
  var aOwn = new owned subthing(3,4);
  var a = aOwn.borrow();

  writeln("Writing ", a);

  var f = openMemFile();
  var w = f.writer(locking=false);

  w.write(a);
  w.close();

  var r = f.reader(locking=false);

  var bOwn = new owned subthing(5,6);
  var b = bOwn.borrow();

  r.read(b);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
  assert(a.y == b.y);
}

{
  var aOwn = new owned subthing(3,4);
  var a = aOwn.borrow();

  writeln("Writing ", a);

  var f = openMemFile();
  var w = f.writer(locking=false);

  w.write(a);
  w.close();

  var r = f.reader(locking=false);

  var bOwn = new owned subthing(5,6);
  var b = bOwn.borrow();
  var c:borrowed mything = b;

  r.read(c);
  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
  assert(a.y == b.y);
}


