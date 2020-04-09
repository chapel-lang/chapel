prototype module OuterModule {
  // Example from Louis Jenkins in #10805
  // Covers error message when non-throws function calls a throwing function
  // from within a coforall statement
  module M {
          proc throwingFn() throws {}
          proc badFn() {
                  coforall i in 1..10 {
                          throwingFn();
                  }
          }
  }

  use M;
  badFn();
}
