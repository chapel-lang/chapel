proc main {
  var s$: sync int = 0;

  s$ += 1;
  s$ *= 2;
  s$ **= 3;
  s$ >>= 2;
  s$ -= 1;
  writeln(s$.readFF());
}
