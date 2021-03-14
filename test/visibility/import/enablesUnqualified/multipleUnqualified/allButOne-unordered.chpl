// Ensures we can access all the symbols in the import's list for unqualified
// access, even if they aren't in declaration order
module A {
  var a: int;
  var b: bool;
  var c = 11;
  var d = "blah blah";
}

module User {
  import A.{a, c, b};

  proc main() {
    writeln(a);
    writeln(b);
    writeln(c);
  }
}
