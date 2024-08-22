use Regex;

proc test(re) {
  writeln("bb".replace(re, "a"));
  writeln("bb".replaceAndCount(re, "a"));
  writeln("count=0 ", "bb".replaceAndCount(re, "a", count=0));
  writeln("count=1 ", "bb".replaceAndCount(re, "a", count=1));
  writeln("count=2 ", "bb".replaceAndCount(re, "a", count=2));
  writeln("count=3 ", "bb".replaceAndCount(re, "a", count=3));
  writeln("count=4 ", "bb".replaceAndCount(re, "a", count=4));
}

writeln("replacing bb b->a"); // python re.sub(r'b', 'a', "bb") -> aa
test(new regex("b"));

writeln("replacing bb ''->a"); // python re.sub(r'', 'a', "bb") -> ababa
test(new regex(""));

writeln("replacing bb b|->a"); // re.sub(r'b|', 'a', "bb") -> aaa
test(new regex("b|"));

writeln("replacing bb |b->a"); // re.sub(r'|b', 'a', "bb") -> aaaaa
test(new regex("|b"));
