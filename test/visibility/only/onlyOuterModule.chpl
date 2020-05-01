module OuterModule {
  module Outer {
    module Toolbox {
      var x = 1;
    }
  }

  proc main() {
    use Outer.Toolbox only Outer; // This should not compile
  }
}
