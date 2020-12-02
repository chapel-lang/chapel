// Test variations of atomic compareExchange, which has slightly strange
// implementations since there are 2 "return values" (success and expected
// getting updated on failure).

proc testCompareExchange(type t) {
  var a: atomic t;
  var expected: t;
  // test local atomic and local expected
  testCompareExchangeHelp(t, a, expected);

  // test remote atomic and remote expected
  on Locales[numLocales-1] {
    testCompareExchangeHelp(t, a, expected);
  }

  // test remote atomic and local expected
  on Locales[numLocales-1] {
    var expectedLocal: t;
    testCompareExchangeHelp(t, a, expectedLocal);
  }

  // test local atomic and remote expected
  on Locales[numLocales-1] {
    var expectedRemote: t;
    on Locales[0] {
      testCompareExchangeHelp(t, a, expectedRemote);
    }
  }

}

proc testCompareExchangeHelp(type t, ref a: atomic t, ref expected: t) {
  a.write(0:t);
  expected = 1:t;
  var b = false;
  writeln((b, a, expected));
  b = a.compareExchange(expected, 1:t);
  writeln((b, a, expected));
  b = a.compareExchange(expected, 1:t);
  writeln((b, a, expected));
  writeln();
}

testCompareExchange(bool);

testCompareExchange(int(8));
testCompareExchange(int(16));
testCompareExchange(int(32));
testCompareExchange(int(64));

testCompareExchange(uint(8));
testCompareExchange(uint(16));
testCompareExchange(uint(32));
testCompareExchange(uint(64));

testCompareExchange(real(32));
testCompareExchange(real(64));
