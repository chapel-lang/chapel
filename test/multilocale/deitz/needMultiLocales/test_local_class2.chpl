class C {
  var x = 17;
}

proc main {
  var c = new C(18);
  writeln(here.id, " ", c.x);
  on Locales(1) {
    c = new C(19);
    writeln(here.id, " ", c.x);
  }
  writeln(here.id, " ", c.x);
}
