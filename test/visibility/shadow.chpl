module A {

  proc shadow() {
    writeln('A scope');
  }

  proc newline() { writeln(); }

  record R {
    proc shadow() {

      writeln('R scope');

      // These work fine
      A.newline();
      newline();

      // This does not
      A.shadow();
    }
  }
}


proc main() {
  use A;
  var r = new R();

  r.shadow();
}
