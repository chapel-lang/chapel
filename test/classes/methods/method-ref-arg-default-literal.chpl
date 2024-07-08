class C {
  proc foo(ref x = 7) {
    x += 1;
    writeln(x);
  }
}

var c = new C();
c.foo();
