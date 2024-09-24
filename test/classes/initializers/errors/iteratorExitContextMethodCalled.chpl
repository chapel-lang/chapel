class IteratorExitContext {
  proc enterContext() {}

  iter exitContext() {}
}

proc main() {
  var i = new IteratorExitContext();
  manage i {
    writeln("blah blah");
  }
}
