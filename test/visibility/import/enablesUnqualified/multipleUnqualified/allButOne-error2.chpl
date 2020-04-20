// Ensures we can't access any symbols for qualified access
module A {
  var a: int;
  var b: bool;
  var c = 11;
  var d = "blah blah";
}

module User {
  import A.{a, c, b};

  proc main() {
    writeln(A.b);
  }
}
