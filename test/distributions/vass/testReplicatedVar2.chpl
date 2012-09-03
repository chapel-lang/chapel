// This test invokes the test examples provided with the implementation
// of the replicated variables.

use ReplicatedDist;

rcExample(5.0, 33, rootLocale.getLocale(0));

if (numLocales >= 4) {
  writeln();
  rcExample(6.1, 44, rootLocale.getLocale(2));
  writeln();
  rcExampleOverLocales(7.6, 55, rootLocale.getLocale(3), rootLocale.getLocales()(1..3 by 2));
}
