module M {
  var x: int;
}
module User {
  import M.{x as foo()}; // Should fail, only identifiers allowed as new names

  proc main() {
    writeln(blah);
  }

  proc foo() param {
    return "blah";
  }
}
