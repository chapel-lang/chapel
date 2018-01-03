module M1 {
  record R {}

  proc foo(x = new R()) {
    writeln(x);
  }
}

module M2 {
  proc main {
    use M1 only foo;
    foo();
  }
}
