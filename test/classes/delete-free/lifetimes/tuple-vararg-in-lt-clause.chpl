// This is an extract from
//   test/library/packages/UnitTest/UnitTest.chpl
// when its proc dependsOn is invoked from one of:
//   test/library/packages/UnitTest/test_locales.chpl
//   test/library/packages/UnitTest/test_dependency.chpl

class Test {
  proc dependsOn(tests: object...)
    lifetime this < tests
  { }
  proc dependsOnX(tests: object...)
    lifetime this < tests
  { writeln(tests(1)); }
  proc dependsOnY(tests: object...)
    lifetime this < tests
  { var quests = tests; writeln(quests); }
}

const obj = new object();
const ttt = new Test();
ttt.dependsOn(obj);
ttt.dependsOnX(obj);
ttt.dependsOnY(obj);
