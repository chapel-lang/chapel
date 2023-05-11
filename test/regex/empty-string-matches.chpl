// Verify that `matches` produces at most one match
// when `text` is an empty string.

use Regex;

proc test(str: string) {
  writeln((new regex("")).matches(str));
  writeln((new regex("^")).matches(str));
  writeln((new regex("$")).matches(str));
  writeln((new regex("^$")).matches(str));
  writeln((new regex(".*")).matches(str));
}

test("");
writeln();
test("xy");
