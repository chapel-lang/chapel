module M1 {
  _debugWriteln(localeID(), " initializing M1");
  var x: int = 17;
  _debugWriteln(localeID(), " in M1 initialization x = ", x);
}

module M2 {
  _debugWriteln(localeID(), " initializing M2");
  def main() {
    _debugWriteln(localeID(), " starting main");
    on Locales(1) {
      _debugWriteln(localeID(), " using M1");
      use M1;
      _debugWriteln(localeID(), " in main x = ", x);
    }
    _debugWriteln(localeID(), " finishing main");
  }
}
