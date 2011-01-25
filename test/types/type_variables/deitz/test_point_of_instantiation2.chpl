module M1 {
  use M2;
  proc foo() {
    writeln("foo");
    bar(1);
  }
  proc goo() {
    writeln("goo");
  }
  proc main() {
    foo();
  }
}

module M2 {
  proc bar(param p: int) {
    writeln("bar");
    goo();
  }
}

