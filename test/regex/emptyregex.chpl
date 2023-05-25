use Regex;

var r = new regex("");
var s = "one";
assert(r.match(s).matched);
assert(!r.fullMatch(s).matched);
assert(r.search(s).matched);

var matches = r.matches(s);
assert(matches.size == (s.size + 1));
writeln(matches.size);
for (i, (match,)) in zip(matches.domain, matches) {
  writeln("offset=", match.byteOffset);
  assert(i == match.byteOffset);
}

writeln();

var re = new regex("a");
for x in re.split("babbbba") do
  writef("%t\n", x);

writeln();

re = new regex("z");
for x in re.split("babbbba") do
  writef("%t\n", x);

writeln();
for x in r.split(s) do
  writef("%t\n", x);

writeln();
writeln("one".replaceAndCount(r, "A"));
