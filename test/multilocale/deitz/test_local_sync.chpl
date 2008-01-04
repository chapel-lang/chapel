def main {
  var s: sync int;

  s = 2;

  on Locales(1) {
    var i = s;
    _debugWriteln(localeID(), " ", i);
  }

  s = 3;

  on Locales(1) {
    var i = s;
    _debugWriteln(localeID(), " ", i);
  }
}
