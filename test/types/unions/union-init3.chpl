union U {
  var x: int;
  var y: real;
}
proc U.init(y:real) {
  this.y = y;
}

proc main() {
  var uu = new U(y=20.0);
  writeln(uu);
}
