def main {
  var x: int = 17;
  _debugWriteln(localeID(), " x=", x);
  x += 1;
  on Locales(1) {
    _debugWriteln(localeID(), " x=", x);
    x += 1;
    on Locales(0) {
      _debugWriteln(localeID(), " x=", x);
      x += 1;
    }
    _debugWriteln(localeID(), " x=", x);
  }
  _debugWriteln(localeID(), " x=", x);
}
