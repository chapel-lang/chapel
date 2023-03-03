use Regex;
use IO;

proc test(str, pattern, repl, count) {
  var r = new regex(pattern);
  writeln("%s.replaceAndCount(%s, %s)".format(str, pattern, repl));
  const base = str.replaceAndCount(r, repl);
  // these two are testing the fast path
  writeln("Default: ", base);
  writeln("count=1: ", str.replaceAndCount(r, repl, count=1));

  // these two are testing the slow path. The first one is effectively doing the
  // same thing as "Default" above.
  writeln("count=", base[1], ": ",  str.replaceAndCount(r, repl, count=base[1]));
  writeln("count=", count, ": ", str.replaceAndCount(r, repl, count=count));
  writeln();
}

test("", "", "X", count=0);

test("foo", "o", "X", count=0);
test("foo", "o", "X", count=-123);

test("11111", "1+", "0", count=2);
test("11a11", "1+", "0", count=3);
test("1a111", "1+", "0", count=3);
test("11a11"*2, "1+", "0", count=2);

test("banana", "ana", "X", count=2);
