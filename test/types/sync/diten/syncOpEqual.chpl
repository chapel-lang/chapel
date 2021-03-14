proc main {
  var s$: sync int = 0;

  s$.writeEF(s$.readFE() + 1);
  s$.writeEF(s$.readFE() * 2);
  s$.writeEF(s$.readFE() ** 3);
  s$.writeEF(s$.readFE() >> 2);
  s$.writeEF(s$.readFE() - 1);
  writeln(s$.readFF());
}
