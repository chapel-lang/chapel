// This test ensures that lifetime constraints involving varargs
// work properly. It was motivated by
//   modules/packages/UnitTest.chpl
// when its proc dependsOn had a lifetime clause and was invoked from
// one of:
//   test/library/packages/UnitTest/test_locales.chpl
//   test/library/packages/UnitTest/test_dependency.chpl

class Test {
  proc dependsOn(tests: object...)
    lifetime this < tests
  { }                                       // varargs not mentioned in body
  proc dependsOnX(tests: object...)
    lifetime this < tests
  { writeln(tests(0)); }                    // access a single vararg
  proc dependsOnY(tests: object...)
    lifetime this < tests
  { var quests = tests; writeln(quests); }  // access all varargs collectively
}

const obj = (new owned object()).borrow();
const jbo = (new owned object()).borrow();
{
const ttt = (new owned Test()).borrow();
ttt.dependsOn(obj);
ttt.dependsOnX(obj);
ttt.dependsOnY(obj);
ttt.dependsOn(obj,jbo);
ttt.dependsOnX(obj,jbo);
ttt.dependsOnY(obj,jbo);
}
