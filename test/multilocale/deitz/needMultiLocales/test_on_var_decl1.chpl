def main {
  on Locales(1) var x = 12;
  _debugWriteln(here.id, " x=", x);
  on Locales(1) {
    _debugWriteln(here.id, " x=", x);
  }
}
