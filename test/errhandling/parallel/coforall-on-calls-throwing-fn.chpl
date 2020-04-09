prototype module OuterModule {
  // Example from Louis Jenkins in #10805
  // Covers error message when non-throws function calls a throwing function
  // from within a coforall+on statement
  module M {
          proc throwingFn() throws {}
          proc badFn() {
                  coforall loc in Locales do on loc {
                          throwingFn();
                  }
          }
  }

  use M;
  badFn();
}
