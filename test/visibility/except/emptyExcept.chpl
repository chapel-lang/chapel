module M1 {
  var a: int = 12;

  var b: bool = true;
}

module M2 {
  use M1 except ;

  // Should not compile.  I expect a syntax error
  proc main() {
    writeln("Wheeeeeeeeeee");
  }
}
