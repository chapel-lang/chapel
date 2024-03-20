use IO;

class mything : writeSerializable {
  var x:int;
  var y:int;
  override proc serialize(writer, ref serializer) throws {
    writer.writeln(x, " ", y);
  }
  // no readThis. Expect a compile-time error
}

{
  var a = new mything(1);

  writeln("Writing ", a);

  var f = openMemFile();
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
