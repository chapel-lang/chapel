import Regex;

var text = "foo 123 bar 345";

var r = Regex.compile("\\d+");

for m in r.matches(text, maxMatches=1) {
  writeln(m);
}
