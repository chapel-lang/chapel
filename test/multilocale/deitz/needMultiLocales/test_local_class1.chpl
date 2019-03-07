class C {
  var x = 17;
}

proc main {
  var c = new borrowed C();
  writeln(here.id, " ", c.x);
  c.x = 18;
  on Locales(1) {
    writeln(here.id, " ", c.x);
    c.x = 19;
  }
  writeln(here.id, " ", c.x);
}
