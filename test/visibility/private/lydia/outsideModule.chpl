module M3 {
  // since M2 is a private module, it is not visible outside of moduleUse.chpl
  // Therefore, this should error.
  use M2;

  proc main() {
    var x = 16;
    writeln(secretFunction(x));
  }
}
