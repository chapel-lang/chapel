def main {
  var x: int = 17;
  var y: int = 23;
  _debugWriteln(here.id, " x=", x);
  _debugWriteln(here.id, " y=", y);
  x = x + 1;
  y = y + 1;
  on Locales(1) {
    _debugWriteln(here.id, " x=", x);
    _debugWriteln(here.id, " y=", y);
    x = x + 1;
    y = y + 1;
  }
  _debugWriteln(here.id, " x=", x);
  _debugWriteln(here.id, " y=", y);
}
