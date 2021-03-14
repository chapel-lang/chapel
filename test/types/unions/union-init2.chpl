union U {
  var x: int;
  var y: real;
}

proc main() {
  var uu = new U(y=20.0);
  writeln(uu);
}
