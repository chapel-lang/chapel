class C {
  var x = 17;
}

proc main {
  var c = new C();
  _debugWriteln(here.id, " ", c.x);
  c.x = 18;
  on Locales(1) {
    _debugWriteln(here.id, " ", c.x);
    c.x = 19;
  }
  _debugWriteln(here.id, " ", c.x);
}
