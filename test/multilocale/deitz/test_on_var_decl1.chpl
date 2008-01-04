def main {
  on Locales(1) var x = 12;
  _debugWriteln(localeID(), " x=", x);
  on Locales(1) {
    _debugWriteln(localeID(), " x=", x);
  }
}
