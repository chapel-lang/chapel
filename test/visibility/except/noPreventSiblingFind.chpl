module M1 {
  var a: int = 12;

  var b: bool = true;

}

module M2 {
  use M1 except b;

  // Ensures that when we specify we don't want b, we can still see a
  proc main () {
    writeln(a);
  }
}
