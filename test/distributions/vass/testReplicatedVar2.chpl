// This test invokes the test examples provided with the implementation
// of the replicated variables.

use ReplicatedVar;

rcExample(5.0, 33, Locales[0]);

if (numLocales >= 4) {
  writeln();
  rcExample(6.1, 44, Locales[2]);
  writeln();
  rcExampleOverLocales(7.6, 55, Locales[3], Locales(1..3 by 2));
}
