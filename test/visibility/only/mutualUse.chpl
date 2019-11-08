// Ensures that the addition of the 'only' keyword doesn't cause infinite
// recursion when two modules publicly use each other with an 'only' list.
module A {
  public use B only b;

  var a = 15 + 3i;

  proc c () {
    writeln("wheeee");
  }
}

module B {
  public use A only a;

  proc b () {
    writeln("naw, man");
  }

  var d: bool;
}

module M {
  public use A;

  proc main() {
    c();
    writeln(a);
    b();
  }
}
