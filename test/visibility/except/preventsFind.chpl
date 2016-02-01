module M1 {
  var a: int = 12;

  var b: bool = true;

}

module M2 {
  use M1 except b;

  // Ensures that when we specify we don't want b, we actually can't see b
  proc main () {
    if (b) {
      writeln("Here we are, trying to access the very symbol we said we didn't want!");
    } else {
      writeln("We still accessed it!  What fools we are!");
    }
  }
}
