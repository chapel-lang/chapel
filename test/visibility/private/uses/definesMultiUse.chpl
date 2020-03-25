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
    var e: int;

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
    if (c && e > 3) {
      writeln(a - e);
    } else {
      writeln(a*e);
    }
  }
}
