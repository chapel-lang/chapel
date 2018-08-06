extern proc foo();

extern record bar {
  var foo: real;
}

// Use 'main' to ensure that 'bar' is present in the generatec C for this
// module. If it's global, it will be in the header file where this test's
// prediff will not look.
proc main() {
  extern var baz: bar;
  var b: bar;

  foo();
  writeln(baz);
  writeln(baz.foo);
  b.foo = 1.2;
  writeln(b);
}
