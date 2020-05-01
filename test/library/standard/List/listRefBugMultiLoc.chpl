//
// Tests to make sure the bug fix introduced in #15369 works in a multi-
// locale environment.
//
use List;

var l: list(int);
l.append(1);
l.append(2);
writeln(l);
on Locales[numLocales-1] {
  l.first() = 3;
  l.last() = 4;
  writeln(l);
}
writeln(l);
