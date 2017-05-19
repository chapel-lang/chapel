module A {

  proc shadow() {
    writeln('A scope');
  }

  proc other() { }

  record R {
    proc shadow() {

      writeln('R scope');

      // This works
      A.other();
      other();

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
