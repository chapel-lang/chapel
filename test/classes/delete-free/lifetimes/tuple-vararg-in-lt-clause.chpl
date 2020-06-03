// This test ensures that lifetime constraints involving varargs
// work properly. It was motivated by
//   test/library/packages/UnitTest/UnitTest.chpl
// when its proc dependsOn is invoked from one of:
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

const obj = new borrowed object();
const jbo = new borrowed object();
{
const ttt = new borrowed Test();
ttt.dependsOn(obj);
ttt.dependsOnX(obj);
ttt.dependsOnY(obj);
ttt.dependsOn(obj,jbo);
ttt.dependsOnX(obj,jbo);
ttt.dependsOnY(obj,jbo);
}
