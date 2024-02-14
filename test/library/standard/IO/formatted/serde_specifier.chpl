use IO, IO.FormattedIO;

record R : writeSerializable, readDeserializable {
  var x: int;

  proc serialize(writer, ref serializer) throws {
    writer.write("<", x, ">");
  }

  proc ref deserialize(reader, ref deserializer) throws {
    reader.readLiteral("<");
    this.x = reader.read(int);
    reader.readLiteral(">");
  }
}

var f = open("test.txt", ioMode.cwr),
    r = new R(1);

f.writer(locking=false).writef("an R: %?\n", r);

var r2 = new R(2);
f.reader(locking=false).readf("an R: %?\n", r2);

writeln(r == r2);
