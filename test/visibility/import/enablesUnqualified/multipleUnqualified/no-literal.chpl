module M {
  var blah: int;
}

module User {
  import M.{"blah"}; // expected to fail

  proc main() {
    writeln(blah);
  }
}
