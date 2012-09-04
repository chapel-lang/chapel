proc main {
  var s, t: sync int;
  var x, y: int;
  begin {
    t.readFE();
    x = 2;
    y = 2;
    s = 1;
  }
  writeln(here.id, " x=", x, " y=", y);
  t = 1;
  s.readFE();
  writeln(here.id, " x=", x, " y=", y);
  on rootLocale.getLocale(1) {
    writeln(here.id, " x=", x, " y=", y);
    x = 3;
    y = 3;
  }
  writeln(here.id, " x=", x, " y=", y);
}
