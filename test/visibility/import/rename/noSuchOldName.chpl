module M {
  var bar: bool;
}
module User {
  import M.{baz as foo}; // Should fail, baz doesn't exist!

  proc main() {
    writeln(foo);
  }
}
