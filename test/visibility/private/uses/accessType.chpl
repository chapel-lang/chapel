use User; // from includesType.chpl

// Verifies that you cannot make use of types obtained via another module's
// private use.
var foo: Foo = new Foo();
writeln(foo.a);
