class IteratorPostinit {
  iter postinit() {}
}

proc main() {
  var i = new IteratorPostinit();
  writeln(i);
}
