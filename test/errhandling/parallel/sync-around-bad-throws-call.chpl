prototype module OuterModule {
  // Courtesy of Louis Jenkins via #10805
  // Tracks a non-throwing function using a sync around a coforall+on that calls
  // a throwing function
  module M {
    proc throwingFn() throws {}
    proc badFn() {
      sync coforall loc in Locales do on loc {
        throwingFn();
      }
    }
  }

  use M;
  badFn();
}
