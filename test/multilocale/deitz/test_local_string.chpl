def main {
  var x: string = "hello world";
  _debugWriteln(localeID(), " x=", x);
  x = x + 1;
  on Locales(1) {
    _debugWriteln(localeID(), " x=", x);
    x = x + 1;
  }
  _debugWriteln(localeID(), " x=", x);
}
