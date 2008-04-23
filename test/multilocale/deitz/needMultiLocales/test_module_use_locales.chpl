module M1 {
  _debugWriteln(here.id, " initializing M1");
  var x: int = 17;
  _debugWriteln(here.id, " in M1 initialization x = ", x);
}

module M2 {
  _debugWriteln(here.id, " initializing M2");
  def main() {
    _debugWriteln(here.id, " starting main");
    on Locales(1) {
      _debugWriteln(here.id, " using M1");
      use M1;
      _debugWriteln(here.id, " in main x = ", x);
    }
    _debugWriteln(here.id, " finishing main");
  }
}
