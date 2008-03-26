class C {
  var x = 17;
}

def main {
  var c = new C(18);
  _debugWriteln(here.id, " ", c.x);
  on Locales(1) {
    c = new C(19);
    _debugWriteln(here.id, " ", c.x);
  }
  _debugWriteln(here.id, " ", c.x);
}
