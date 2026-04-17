use UnitTest;

import myDep;

proc testStr(test: borrowed Test) throws {
  var (s1, s2) = myDep.getStrings();
  test.assertEqual(s1, "set");
  test.assertEqual(s2, "set");
}

proc testMagic(test: borrowed Test) throws {
  var magic = myDep.getMagic();
  test.assertEqual(magic, 17);
}

UnitTest.main();
