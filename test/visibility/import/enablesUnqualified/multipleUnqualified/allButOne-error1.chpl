// Ensures we can't access any unlisted symbols for unqualified access
module A {
  var a: int;
  var b: bool;
  var c = 11;
  var d = "blah blah";
}

module User {
  import A.{a, c, b};

  proc main() {
    writeln(d);
  }
}
