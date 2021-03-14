module A {
  var foo = 13;
}

module B {
  var bar = -2;
  var foo = 34.2;
}

module C {
  var foo = true;
}

module M {
  use A;
  use B except foo;
  use C;
  // Verifies the behavior when more than two uses cause a naming conflict and
  // there is only an 'except' list on one of them.

  proc main() {
    writeln(foo);
  }
}
