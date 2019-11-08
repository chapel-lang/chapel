// This test verifies that if an 'except' list is specified, the symbols are
// ignored in all modules in that use chain, instead of just the module
// specified.  So if some module M uses a module A and excludes 'foo', if A
// publicly uses a module B that also defines a 'foo', that 'foo' will also
// not be visible.

module A {
  public use B;

  var foo = 15; // Should be hidden

  var wanted = 97.3; // Should be visible
}

module B {
  var bar = true; // Should be visible
  var foo = 6; // Should be hidden, too
}

module C {
  public use D;

  var whatev: uint;
}

// Used by C, defines variable which would conflict with B's without 'except'
// specified in the chain which leads to B.
module D {
  var foo = 53; // Should be found without conflicts
}

module M {
  use A except foo;
  use C;

  proc main() {
    writeln(foo); // Expect 53.  Not 15, not 6, not a conflict.
  }
}
