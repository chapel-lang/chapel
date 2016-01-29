// Ensures that infinite recursion doesn't occur when two modules use each
// other, one with an 'only' list and the other with an 'except' list.
module A {
  use B only b;

  var a = 15 + 3i;

  proc c () {
    writeln("wheeee");
  }
}

module B {
  use A except a;

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
