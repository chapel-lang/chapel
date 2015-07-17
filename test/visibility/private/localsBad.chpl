proc foo() {
  private var x = 11; // Should generate warning?  error?

  writeln(x);
  return x;
}

foo();
