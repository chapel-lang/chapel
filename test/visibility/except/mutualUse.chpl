// Ensures that the addition of the 'except' keyword doesn't cause infinite
// recursion when two modules publicly use each other with an 'except' list.
module A {
  public use B except b;

  var a = 15 + 3i;

  proc c () {
    writeln("wheeee");
  }
}

module B {
  public use A except a;

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
    writeln(d);
  }
}
