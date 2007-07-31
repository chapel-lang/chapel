def main {
  var x: int = 17;
  _debugWriteln(localeID(), " x=", x);
  x = x + 1;
  on Locale(1) {
    _debugWriteln(localeID(), " x=", x);
    x = x + 1;
  }
  _debugWriteln(localeID(), " x=", x);
}
