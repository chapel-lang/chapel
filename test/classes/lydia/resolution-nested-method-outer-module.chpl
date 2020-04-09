module other1{
  record foo {
    var t: int = 6;

    proc bar() {
      writeln("foo unmodified");
    }
  }
}

module other2 {
  proc modifyIt() {
    use other1;
    // Modifying foo here affects all calls to it within this
    // scope.
    var baz: foo;
    baz.bar();
    proc foo.bar() {
      writeln("Modified foo");
    }
    baz.bar();
  }

  proc main() {
    use other1;
    // Calling foo.bar here should not be affected by modifyIt
    // shadowing foo.bar within its scope
    var baz: foo;
    baz.bar();
    modifyIt();
    baz.bar();
  }
}
