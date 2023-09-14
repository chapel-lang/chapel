record R : writeSerializable {
  var x: int = 0;
}

var printcopies = true;

proc R.init=(arg: R) {
  if printcopies then
    writeln("copy/assign from ", arg.x);

  this.x = arg.x;
}

operator R.=(ref lhs: R, rhs: R) {
  if printcopies then
    writeln("copy/assign from ", rhs.x);

  lhs.x = rhs.x;
}

proc R.serialize(writer, ref serializer) throws {
  writer.write(x);
}
