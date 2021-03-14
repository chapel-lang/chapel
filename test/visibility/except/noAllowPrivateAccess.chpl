module M1 {
  var a: int = 12;

  var b: bool = true;

  private var c: real = 17.5;

}

module M2 {
  use M1 except b;

  // Ensures that when we specify we don't want b, it doesn't allow us to see
  // the private variable c
  proc main () {
    writeln(c);
  }
}
