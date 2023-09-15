record foo : writeSerializable {
  var x : int = 1;
  var y : int = 2;
  var z : int = 3;
}

proc foo.serialize(writer, ref serializer) throws {
  writer.write("(---", x, " ", y, " ", z, "---)");
}

var f : foo;

writeln(f);
