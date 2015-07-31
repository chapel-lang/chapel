module M3 {
  // since M2 is a private global module, it should be visible
  use M2;

  proc main() {
    var x = 16;
    writeln(secretFunction(x));
  }
}
