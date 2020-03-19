module OuterModule {
  module Toolbox {
    var x = 1;
  }

  proc main() {
    use this.Toolbox except Toolbox; // This should not compile
  }
}
