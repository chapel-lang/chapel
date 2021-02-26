module M {
  var x: int;
}
module User {
  import M.x as foo;

  proc main() {
    writeln(foo.x); // Ensures that we don't accidentally replace M with Foo
  }
}
