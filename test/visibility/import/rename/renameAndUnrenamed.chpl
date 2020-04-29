module A {
  var x: int;
  var y = "blah";
}
module User {
  import A.{x, x as y}; // works but warns user in case they didn't intend to

  proc main() {
    writeln(x); // A.x
    writeln(y); // A.x
  }
}
