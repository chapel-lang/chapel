union U {
  var x: int;
  var y: real;
}

proc main() {
  var uu = new U(x=1, y=20.0);
  writeln(uu);
}
