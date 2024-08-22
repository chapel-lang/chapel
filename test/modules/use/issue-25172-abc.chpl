module A {
  proc main() {
    use C;
    import B.{foo};
    foo();
  }
}
module B {
  public use C;
}

module C {
  proc foo() {
    writeln("Just chilling");
  }
}
