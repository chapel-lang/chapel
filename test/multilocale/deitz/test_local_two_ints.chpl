def main {
  var x: int = 17;
  var y: int = 23;
  _debugWriteln(localeID(), " x=", x);
  _debugWriteln(localeID(), " y=", y);
  x = x + 1;
  y = y + 1;
  on Locales(1) {
    _debugWriteln(localeID(), " x=", x);
    _debugWriteln(localeID(), " y=", y);
    x = x + 1;
    y = y + 1;
  }
  _debugWriteln(localeID(), " x=", x);
  _debugWriteln(localeID(), " y=", y);
}
