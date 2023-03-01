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
