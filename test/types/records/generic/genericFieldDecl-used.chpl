record One {
  var x: record = new Two(2);
}
record Two {
  var y: int;
}

proc main() {
  var foo: One;
  writeln(foo);
}
