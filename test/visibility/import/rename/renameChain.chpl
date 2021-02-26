module M {
  var foo: bool;
}
module User {
  import M.{foo as bar, bar as baz}; // Should fail

  proc main() {
    writeln(bar);
    writeln(baz);
  }
}
