class IteratorDeinit {
  iter deinit() {}
}

proc main() {
  var i = new IteratorDeinit();
  writeln(i);
}
