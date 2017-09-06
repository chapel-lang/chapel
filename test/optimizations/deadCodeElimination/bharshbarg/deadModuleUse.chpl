
//
// This test exists to ensure an old bug does not resurface.
//
// The bug was that when we DCE'd module 'A', we added 'use B' to module B's
// use-list. Then when we DCE'd module 'B', we added that 'use B' to
// deadModuleUse's use-list. This resulted in a failure during codegen because
// a CallExpr for B's initFn was still in the tree despite B being dead.
//

module A {
  use B;
}
module B {
  use A;
}

module deadModuleUse {
  use A;

  proc main() {
    writeln("success");
  }
}
