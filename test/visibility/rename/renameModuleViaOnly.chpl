module OuterModule {
  module Toolbox {
    var x = 1;
  }

  proc main() {
    use Toolbox only Toolbox as tb; // This should not compile
  }
}
