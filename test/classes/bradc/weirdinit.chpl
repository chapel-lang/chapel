class pair {
  var x: int;
  var y: real;
}

proc main() {
  var a: borrowed pair = a();
  writeln("Shouldn't that line have generated an error?");
}
