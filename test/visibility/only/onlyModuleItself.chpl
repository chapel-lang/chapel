module OuterModule {
  module Toolbox {
    var x = 1;
  }

  proc main() {
    use Toolbox only Toolbox; // This should not compile
  }
}
