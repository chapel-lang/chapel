prototype module OuterModule {
  // Courtesy of Louis Jenkins via #10805
  // Tracks a non-throwing function using a try around a sync around a
  // coforall+on that calls a throwing function
  module M {
    proc throwingFn() throws {}
    proc goodFn() {
      try! {
        sync coforall loc in Locales do on loc {
            throwingFn();
        }
      }
    }
  }

  use M;
  goodFn();
}
