module A {
  var x: int;
  var y = "blah";
}
module User {
  import A.{x, y as x}; // should fail, these conflict

  proc main() {
    writeln(x);
  }
}
