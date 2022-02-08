import Regex;

var text = "foo 123 bar 345";

var r = Regex.compile("\\d+");

for m in r.matches(text, captures=0, maxmatches=1) {
  writeln(m);
}
