def main {
  var x: [1..3] int = (4, 5, 6);
  _debugWriteln(localeID(), " x = ", x(1), " ", x(2), " ", x(3));
  x = x + 1;
  on Locales(1) {
    _debugWriteln(localeID(), " x = ", x(1), " ", x(2), " ", x(3));
    x = x + 1;
  }
  _debugWriteln(localeID(), " x = ", x(1), " ", x(2), " ", x(3));
}
