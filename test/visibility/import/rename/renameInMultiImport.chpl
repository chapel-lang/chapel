module A {
  var x: int;
  var y: bool;
}
module User {
  import A.{x as foo};

  proc main() {
    writeln(foo);
  }
}
