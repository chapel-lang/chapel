module M2 {
  proc bar() {
    writeln("bar");
  }
}
module M1 {
  public use M2;

  proc foo() {
    writeln("foo");
  }
}

module M3 {
  use M1;
  proc main() {
    bar();
  }
}
