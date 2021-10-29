use Regex;

var r = compile("");
var s = "one";
assert(r.match(s).matched);
assert(!r.fullMatch(s).matched);
assert(r.search(s).matched);

var matches = r.matches(s);
assert(matches.size == (s.size + 1));
writeln(matches.size);
for (i, (match,)) in zip(matches.domain, matches) {
  writeln("offset=", match.offset);
  assert(i == match.offset);
}

writeln();

for x in compile("a").split("babbbba") do
  writef("%t\n", x);

writeln();

for x in compile("z").split("babbbba") do
  writef("%t\n", x);

writeln();
for x in r.split(s) do
  writef("%t\n", x);

writeln();
writeln(r.subn("A", "one"));