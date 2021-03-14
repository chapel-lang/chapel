module M1 {
  var a: int = 12;

  var b: bool = true;
}

module M2 {
  use M1 except q;

  // Should not compile.  I expect a symbol resolution error
  proc main() {
    writeln("Wheeeeeeeeeee");
  }
}
