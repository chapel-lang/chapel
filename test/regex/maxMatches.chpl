use Regex;

var text = "foo 123 bar 345";

var r = new regex("\\d+");

for m in r.matches(text, maxMatches=1) {
  writeln(m);
}
