use IO;

record xyType {
  var x: int;
  var y: real;
}

proc xyType.readThis(fr) throws {
  fr.matchLiteral("|");
  this.x = fr.read(int);
  fr.matchLiteral(",");
  this.y = fr.read(real);
  fr.matchLiteral("|");
}

proc xyType.init(x = 0, y = 0.0) {
  this.x = x;
  this.y = y;
}

proc xyType.init(reader, ref deserializer) {
  this.init();
  readThis(reader);
}

proc xyType.deserialize(reader, ref deserializer) throws {
  readThis(reader);
}

proc xyType.writeThis(fw) throws {
  fw.write("(", this.x, ", ", this.y, ")");
}

var r = openReader("xy.txt");

// read to the start of the XY input
writeln(r.readTo("|"));

// read an xy
writeln("xy: ", r.read(xyType));

// read to the end of the file
write(r.readTo("|"));
