// Ensures that the addition of the 'only' keyword doesn't cause infinite
// recursion when two modules use each other with an 'only' list.
module A {
  use B only b;

  var a = 15 + 3i;

  proc c () {
    writeln("wheeee");
  }
}

module B {
  use A only a;

  proc b () {
    writeln("naw, man");
  }

  var d: bool;
}

module M {
  use A;

  proc main() {
    c();
    writeln(a);
    b();
  }
}
