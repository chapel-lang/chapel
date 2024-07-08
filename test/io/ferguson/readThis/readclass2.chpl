use IO;

class mything : serializable {
  var x:int;
  var y:int;

  proc init(x: int = 0, y: int = 0) {
    this.x = x;
    this.y = y;
  }
  proc init(reader, ref deserializer) {
    this.x = reader.read(int);
    reader.readLiteral(" ");
    this.y = reader.read(int);
  }

  override proc deserialize(reader, ref deserializer) throws {
    x = reader.read(int);
    reader.readLiteral(" ");
    y = reader.read(int);
  }

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
    writer.writeLiteral(" ");
    writer.write(y);
  }
}

{
  var ownA = new owned mything(1);
  var a = ownA.borrow();

  writeln("Writing ", a);

  var f = openMemFile();
  var w = f.writer(locking=false);

  w.write(a);
  w.close();

  var r = f.reader(locking=false);

  var ownB = new owned mything(2);
  var b = ownB.borrow();
  r.read(b);

  r.close();

  writeln("Read ", b);

  assert(a.x == b.x);
}
