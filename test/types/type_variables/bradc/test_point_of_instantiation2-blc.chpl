module M1 {
  use M2;
  proc foo() {
    writeln("foo");
    bar(1);
  }
  proc goo() {
    writeln("M1's goo");
  }
  proc main() {
    foo();
    baz();
  }
}

module M2 {
  proc goo() {
    writeln("M2's goo");
  }
  proc bar(param p: int) {
    writeln("bar");
    goo();
  }
  proc baz() {
    bar(1);
    bar(2);
    goo();
  }
}

