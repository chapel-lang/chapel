module A { }
module B {
  public use A only;
}
module D {
  use B;
  proc main() { }
}
