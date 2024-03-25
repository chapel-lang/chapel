use IO;

record xyType : serializable {
  var x: int;
  var y: real;
}

proc ref xyType.deserialize(reader, ref deserializer) throws {
  reader.matchLiteral("|");
  this.x = reader.read(int);
  reader.matchLiteral(",");
  this.y = reader.read(real);
  reader.matchLiteral("|");
}

proc xyType.init(x = 0, y = 0.0) {
  this.x = x;
  this.y = y;
}

proc xyType.init(reader, ref deserializer) {
  this.init();
  deserialize(reader, deserializer);
}

proc xyType.serialize(writer, ref serializer) throws {
  writer.write("(", this.x, ", ", this.y, ")");
}

var r = openReader("xy.txt", locking=false);

// read to the start of the XY input
writeln(r.readTo("|"));

// read an xy
writeln("xy: ", r.read(xyType));

// read to the end of the file
write(r.readTo("|"));
