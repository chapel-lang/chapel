def main {
  var x: int = 17;
  _debugWriteln(localeID(), " x=", x);
  x += 1;
  _debugWriteln(localeID(), " x.locale=", x.locale);
  on Locales(1) {
    _debugWriteln(localeID(), " x=", x);
    x += 1;
    _debugWriteln(localeID(), " x.locale=", x.locale);
  }
  _debugWriteln(localeID(), " x=", x);
}
