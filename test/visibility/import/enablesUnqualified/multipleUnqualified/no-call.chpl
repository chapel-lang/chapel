module M {
  var blah: int;
}

module User {
  import M.{foo()}; // expected to fail

  proc foo() param {
    return 2;
  }

  proc main() {
    writeln(blah);
  }
}
