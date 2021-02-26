module A {
  var x: int;
}
module User {
  import A as _; // This should fail

  proc main() {
    writeln("uh oh, I compiled!");
  }
}
