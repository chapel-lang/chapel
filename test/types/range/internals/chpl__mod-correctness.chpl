// 'ChapelRange.chpl__mod(dividend, modulus)' returns 'r' with 0 <= r < abs(modulus)
// congruent to 'dividend' modulo 'modulus', typed like 'dividend'.

use UnitTest;


proc testBasic(test: borrowed Test) throws {
  test.assertEqual(chpl__mod(10, 3), 1);
  test.assertEqual(chpl__mod(100, 7), 2);
  test.assertEqual(chpl__mod(7, 10), 7);
  test.assertEqual(chpl__mod(1, 2), 1);
  test.assertEqual(chpl__mod(2, 2), 0);

  test.assertEqual(chpl__mod(9, 3), 0);
  test.assertEqual(chpl__mod(0, 5), 0);
  test.assertEqual(chpl__mod(-9, 3), 0);
  test.assertEqual(chpl__mod(21, 7), 0);
  test.assertEqual(chpl__mod(-21, 7), 0);

  test.assertEqual(chpl__mod(0, 5), 0);
  test.assertEqual(chpl__mod(0, -5), 0);
  test.assertEqual(chpl__mod(0, 1), 0);
}
proc testSigns(test: borrowed Test) throws {
  test.assertEqual(chpl__mod(-10, 3), 2);
  test.assertEqual(chpl__mod(-1, 3), 2);
  test.assertEqual(chpl__mod(-100, 7), 5);
  test.assertEqual(chpl__mod(-9, 3), 0);

  test.assertEqual(chpl__mod(10, -3), 1);
  test.assertEqual(chpl__mod(7, -10), 7);

  test.assertEqual(chpl__mod(-10, -3), 2);
  test.assertEqual(chpl__mod(-7, -10), 3);

  test.assertEqual(chpl__mod(0, -5), 0);
  test.assertEqual(chpl__mod(123456, 1), 0);
  test.assertEqual(chpl__mod(-5, -1), 0);
}

proc testResultType(test: borrowed Test) throws {
  test.assertTrue(chpl__mod(10:int(8),  3:int(8)).type  == uint(8));
  test.assertTrue(chpl__mod(10:int(16), 3:int(16)).type == uint(16));
  test.assertTrue(chpl__mod(10:int(32), 3:int(32)).type == uint(32));
  test.assertTrue(chpl__mod(10:uint,    3:uint).type    == uint);
  test.assertTrue(chpl__mod(5:int(8),   3:int(64)).type == uint);
}

proc testSignedWidths(test: borrowed Test) throws {
  test.assertEqual(chpl__mod(10:int(8),  3:int(8)),  1:int(8));
  test.assertEqual(chpl__mod(10:int(16), 3:int(16)), 1:int(16));
  test.assertEqual(chpl__mod(10:int(32), 3:int(32)), 1:int(32));
  test.assertEqual(chpl__mod(10:int(64), 3:int(64)), 1:int(64));

  test.assertEqual(chpl__mod(-10:int(8),  3:int(8)),  2:int(8));
  test.assertEqual(chpl__mod(-10:int(16), 3:int(16)), 2:int(16));
  test.assertEqual(chpl__mod(-10:int(32), 3:int(32)), 2:int(32));
  test.assertEqual(chpl__mod(-10:int(64), 3:int(64)), 2:int(64));

  test.assertEqual(chpl__mod(max(int(8)),  10:int(8)),   7:int(8));
  test.assertEqual(chpl__mod(max(int(16)), 100:int(16)), 67:int(16));
  test.assertEqual(chpl__mod(max(int(32)), 7:int(32)),   1:int(32));
  test.assertEqual(chpl__mod(max(int(64)), 3:int(64)),   1:int(64));
  test.assertEqual(chpl__mod(max(int(64)), 2:int(64)),   1:int(64));

  test.assertEqual(chpl__mod(min(int(8)),  10:int(8)),   2:int(8));
  test.assertEqual(chpl__mod(min(int(8)),  3:int(8)),    1:int(8));
  test.assertEqual(chpl__mod(min(int(16)), 100:int(16)), 32:int(16));
  test.assertEqual(chpl__mod(min(int(32)), 7:int(32)),   5:int(32));
  test.assertEqual(chpl__mod(min(int(64)), 3:int(64)),   1:int(64));
  test.assertEqual(chpl__mod(min(int(64)), 2:int(64)),   0:int(64));
  test.assertEqual(chpl__mod(min(int(8)), -3:int(8)),    1:int(8));

  test.assertEqual(chpl__mod(5:int(8),     max(int(8))),  5:int(8));
  test.assertEqual(chpl__mod(127:int(8),   max(int(8))),  0:int(8));
  test.assertEqual(chpl__mod(1000:int(16), max(int(16))), 1000:int(16));
  test.assertEqual(chpl__mod(5:int(32),    max(int(32))), 5:int(32));

  test.assertEqual(chpl__mod(5:int(8),   min(int(8))),   5:int(8));
  test.assertEqual(chpl__mod(100:int(8), min(int(8))), 100:int(8));
  test.assertEqual(chpl__mod(5:int(32),  min(int(32))),  5:int(32));
}

proc testUnsignedWidths(test: borrowed Test) throws {
  test.assertEqual(chpl__mod(10:uint(8),  3:uint(8)),  1:uint(8));
  test.assertEqual(chpl__mod(10:uint(16), 3:uint(16)), 1:uint(16));
  test.assertEqual(chpl__mod(10:uint(32), 3:uint(32)), 1:uint(32));
  test.assertEqual(chpl__mod(10:uint(64), 3:uint(64)), 1:uint(64));

  test.assertEqual(chpl__mod(7:uint(8),   10:uint(8)),  7:uint(8));
  test.assertEqual(chpl__mod(7:uint(64),  10:uint(64)), 7:uint(64));
  test.assertEqual(chpl__mod(9:uint(16),  3:uint(16)),  0:uint(16));
  test.assertEqual(chpl__mod(21:uint(32), 7:uint(32)),  0:uint(32));

  test.assertEqual(chpl__mod(max(uint(8)),  4:uint(8)),  3:uint(8));
  test.assertEqual(chpl__mod(max(uint(16)), 4:uint(16)), 3:uint(16));
  test.assertEqual(chpl__mod(max(uint(32)), 4:uint(32)), 3:uint(32));
  test.assertEqual(chpl__mod(max(uint(64)), 4:uint(64)), 3:uint(64));

  test.assertEqual(chpl__mod(5:uint(8),  max(uint(8))),    5:uint(8));
  test.assertEqual(chpl__mod(max(uint(8)),  max(uint(8))), 0:uint(8));
  test.assertEqual(chpl__mod(5:uint(64), max(uint(64))),   5:uint(64));
}

proc testMixedSign(test: borrowed Test) throws {
  test.assertEqual(chpl__mod(10:uint(8),  3:int(8)),  1:uint(8));
  test.assertEqual(chpl__mod(7:uint(8),   10:int(8)), 7:uint(8));
  test.assertEqual(chpl__mod(10:uint(64), 3:int(64)), 1:uint(64));
  test.assertEqual(chpl__mod(10:uint(8), -3:int(8)),  1:uint(8));
  test.assertEqual(chpl__mod(10:uint(16), 3:int(8)),  1:uint(16));
  test.assertEqual(chpl__mod(10:uint(8),  3:int(16)), 1:uint(8));
  test.assertEqual(chpl__mod(200:uint(8), 3:int(16)), 2:uint(8));

  test.assertEqual(chpl__mod(10:int(8),  3:uint(8)),  1:int(8));
  test.assertEqual(chpl__mod(7:int(8),   10:uint(8)), 7:int(8));
  test.assertEqual(chpl__mod(10:int(64), 3:uint(64)), 1:int(64));
  test.assertEqual(chpl__mod(-10:int(8),  3:uint(8)),  2:int(8));
  test.assertEqual(chpl__mod(-10:int(64), 3:uint(64)), 2:int(64));
  test.assertEqual(chpl__mod(10:int(16),  3:uint(8)),  1:int(16));
  test.assertEqual(chpl__mod(-10:int(16), 3:uint(8)),  2:int(16));

  test.assertEqual(chpl__mod(10:int(8),   3:int(32)), 1:uint(8));
  test.assertEqual(chpl__mod(-10:int(8),  3:int(32)), 2:uint(8));
  test.assertEqual(chpl__mod(200:uint(8), 3:int(16)), 2:uint(8));
}

proc testMinModulusWraparound(test: borrowed Test) throws {
  test.assertEqual(chpl__mod(-1:int(8),   min(int(8))), max(int(8)));
  test.assertEqual(chpl__mod(-100:int(8), min(int(8))), max(int(8)) - 99);
  test.assertEqual(chpl__mod(-127:int(8), min(int(8))), 1:int(8));
  test.assertEqual(chpl__mod(min(int(8)), min(int(8))), 0:int(8));

  test.assertEqual(chpl__mod(-1:int(32),   min(int(32))), max(int(32)));
  test.assertEqual(chpl__mod(-5:int(32),   min(int(32))), max(int(32)) - 4);
  test.assertEqual(chpl__mod(min(int(64)), min(int(64))), 0:int(64));

  test.assertEqual(chpl__mod(5:int(8),   min(int(8))),   5:int(8));
  test.assertEqual(chpl__mod(100:int(8), min(int(8))), 100:int(8));
}

// Residue invariant: for many dividend/modulus pairs the result is in
// [0, abs(modulus)) and congruent to the dividend.
proc testResultInvariant(test: borrowed Test) throws {
  for d in -50..50 {
    for m in [2, 3, 5, 7, 13, -4, -6, -9] {
      const r = chpl__mod(d, m);
      const am = abs(m);
      test.assertTrue(r >= 0 && r < am);
      test.assertEqual((d - r:int) % am, 0);
    }
  }

  // Same invariant for the min(t) wraparound path. |min| is not representable,
  // so congruence is checked in a wider type.
  for k in 1..130 {
    const d = (-k):int(8);
    const r = chpl__mod(d, min(int(8)));
    test.assertTrue(r >= 0);
    test.assertEqual((r:int(32) - d:int(32)) % 128, 0);
  }
}

UnitTest.main();
