def main {
  var x: string = "hello world";
  _debugWriteln(here.id, " x=", x);
  x = x + 1;
  on Locales(1) {
    _debugWriteln(here.id, " x=", x);
    x = x + 1;
  }
  _debugWriteln(here.id, " x=", x);
}
