module definesMultiUse {
  module Inner1 {
    var a = 15;

    proc b() {
      writeln("Something interesting");
    }
  }

  module Inner2 {
    var c = false;

    proc d() {
      writeln("Obviously this is useful or you wouldn't be calling it");
    }
  }

  module Inner3 {
    var ee: int;

    proc f() {
      writeln("and this as well");
    }
  }

  private use Inner1, Inner2, Inner3;

  // Ensures that a private use doesn't interfere with our ability to access the
  // used modules' symbols
  proc main() {
    b();
    d();
    f();
    if (c && ee > 3) {
      writeln(a - ee);
    } else {
      writeln(a*ee);
    }
  }
}
