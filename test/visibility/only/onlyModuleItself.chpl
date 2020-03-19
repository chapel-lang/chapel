module OuterModule {
  module Toolbox {
    var x = 1;
  }

  proc main() {
    use this.Toolbox only Toolbox; // This should not compile
  }
}
