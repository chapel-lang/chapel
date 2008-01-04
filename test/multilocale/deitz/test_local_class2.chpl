class C {
  var x = 17;
}

def main {
  var c = C(18);
  _debugWriteln(localeID(), " ", c.x);
  on Locales(1) {
    c = C(19);
    _debugWriteln(localeID(), " ", c.x);
  }
  _debugWriteln(localeID(), " ", c.x);
}
