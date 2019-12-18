module OuterModule {
  module M {
    proc uhoh() {
      module realBad {
      }
    }
  }

  uhoh();
}
