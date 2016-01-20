class C {
  private var foo = 13;

  private proc bar() {
    return 10;
  }
}

var c: C = new C();
writeln(c.foo);
writeln(c.bar());
