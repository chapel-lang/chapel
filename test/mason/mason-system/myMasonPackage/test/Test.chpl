import myMasonPackage;
import myDep;
use UnitTest;

proc testPackage(test: borrowed Test) throws {
  const flag = myMasonPackage.get();
  test.assertEqual(flag, 2);
}
proc testDep(test: borrowed Test) throws {
  const (str, num) = myDep.get();
  test.assertClose(str, 3.14);
  test.assertEqual(num, 17);
}

UnitTest.main();
