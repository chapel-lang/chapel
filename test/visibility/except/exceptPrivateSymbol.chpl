module M1 {
  var a: int = 12;

  private var b: bool = true;
}

module M2 {
  use M1 except b;

  // Should not compile.  I expect a symbol resolution error
  // "b?  What b? I don't see any b in here . . ."
  proc main() {
    writeln("Wheeeeeeeeeee");
  }
}
