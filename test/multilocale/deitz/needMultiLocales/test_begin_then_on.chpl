def main {
  var s, t: sync int;
  var x, y: int;
  begin {
    t.readFE();
    x = 2;
    y = 2;
    s = 1;
  }
  _debugWriteln(here.id, " x=", x, " y=", y);
  t = 1;
  s.readFE();
  _debugWriteln(here.id, " x=", x, " y=", y);
  on Locales(1) {
    _debugWriteln(here.id, " x=", x, " y=", y);
    x = 3;
    y = 3;
  }
  _debugWriteln(here.id, " x=", x, " y=", y);
}
