import myMasonPackage;
import myDep;
use UnitTest;

proc testPackage(test: borrowed Test) throws {
  const flag = myMasonPackage.get();
  test.assertEqual(flag, 2);
}
proc testDep(test: borrowed Test) throws {
  const (num1, num2) = myDep.get();
  const num3 = myDep.get2();
  test.assertClose(num1, 3.14);
  test.assertEqual(num2, 17);
  test.assertEqual(num3, 1742);
}

UnitTest.main();
