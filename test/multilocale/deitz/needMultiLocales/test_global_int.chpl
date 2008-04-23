var x: int = 17;

def main {
  _debugWriteln(here.id, " x=", x);
  x += 1;
  on Locales(1) {
    _debugWriteln(here.id, " x=", x);
    x += 1;
  }
  _debugWriteln(here.id, " x=", x);
}
