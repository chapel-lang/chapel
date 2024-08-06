module OuterModule {
  module M {
    proc uhoh() {
      module realBad {
      }
    }
  }

  use M;
  uhoh();
}
