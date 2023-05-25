module A {
  module B {
  }
}

module Main {
  proc main() {
    use A;
    A.B();
  }
}
