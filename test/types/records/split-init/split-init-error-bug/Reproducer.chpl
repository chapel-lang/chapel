module Reproducer {
  use Foo;
  use FooUser;
  proc main() {
      var myFoo : foo(false);
      var retDom : domain(1) = {1..10};
      var retArr : [retDom] real(32);
      //There is only one CallExpr in Reproducer.main, so the loop seems to trigger on this
      Use_Foo(foo(false), myFoo, real(32), retArr);
  }
}
