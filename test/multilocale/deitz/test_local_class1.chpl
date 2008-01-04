class C {
  var x = 17;
}

def main {
  var c = C();
  _debugWriteln(localeID(), " ", c.x);
  c.x = 18;
  on Locales(1) {
    _debugWriteln(localeID(), " ", c.x);
    c.x = 19;
  }
  _debugWriteln(localeID(), " ", c.x);
}
