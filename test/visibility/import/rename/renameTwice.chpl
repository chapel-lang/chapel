module M {
  var foo: bool;
}
module User {
  import M.{foo as bar, foo as baz}; // Should work, but warn just in case

  proc main() {
    writeln(bar);
    writeln(baz);
  }
}
