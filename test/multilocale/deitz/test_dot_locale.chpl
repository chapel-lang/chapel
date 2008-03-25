def main {
  var x: int = 17;
  _debugWriteln(localeID(), " x=", x);
  x += 1;
  _debugWriteln(localeID(), " x.locale.id=", x.locale.id);
  on Locales(1) {
    _debugWriteln(localeID(), " x=", x);
    x += 1;
    _debugWriteln(localeID(), " x.locale.id=", x.locale.id);
  }
  _debugWriteln(localeID(), " x=", x);
}
