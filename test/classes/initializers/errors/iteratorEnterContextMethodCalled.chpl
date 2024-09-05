class IteratorEnterContext {
  iter enterContext() {}

  proc exitContext() {}
}

proc main() {
  var i = new IteratorEnterContext();
  manage i {
    writeln("blah blah");
  }
}
