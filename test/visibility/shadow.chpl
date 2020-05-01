module OuterModule {
  /* Test to ensure that functions shadowed by methods can be called with
     module-access-expression */
  module A {

    proc shadow() {
      writeln('A scope');
    }

    proc newline() { writeln(); }

    record R {
      proc shadow() {

        writeln('R scope');

        A.newline();
        newline();

        A.shadow();
      }
    }
  }


  proc main() {
    use A;
    var r = new R();

    r.shadow();
  }
}
