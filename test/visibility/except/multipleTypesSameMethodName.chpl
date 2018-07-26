module M {
  class C1 {
    var x: int;

    proc foo() {
      writeln(x);
    }
  }

  class C2 {
    var y: bool;

    proc foo() {
      writeln(y);
    }
  }
}

module OtherMod {
  use M except C2;

  proc main() {
    var c1 = new unmanaged C1(7);
    // verifies that if an excluded type has a method with
    // the same name as a method on an included type, the
    // included type can still call its own method.
    c1.foo();
    delete c1;
  }
}
