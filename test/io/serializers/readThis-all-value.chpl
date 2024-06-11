
use IO;

record B : serializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc init(x: int = 0) {
    this.x = x;
  }

  proc init(reader, ref deserializer) throws {
    this.init();
    writeln("B.init");
    this.x = reader.read(int);
  }

  proc ref deserialize(reader, ref deserializer) throws {
    writeln("B.deserialize");
    x = reader.read(int);
  }
}

record A {
  var b : B;
}

proc main() {
  var a : A;
  var f = openMemFile();
  f.writer(locking=false).write(a);

  var x : A;
  f.reader(locking=false).read(x);
  writeln(x);
}
