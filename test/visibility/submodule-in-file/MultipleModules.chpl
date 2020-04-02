// This case actually compiles as long as the
// module doing the including is named appropriately.
module MultipleModules {
  include module SubModule;
  proc main() {
  }
}
module OtherModule {
  var otherVar = 1;
  proc otherFn() { return 2; }
}
