module M {
  var x: int;
}
module User {
  import M.{"x" as foo}; // Should fail, only identifiers allowed as old names

  proc main() {
    writeln(foo);
  }
}
