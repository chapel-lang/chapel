class C {
  var x = 17;
}

proc main {
  var c = new unmanaged C(18);
  writeln(here.id, " ", c.x);
  on Locales(1) {
    delete c;
    c = new unmanaged C(19);
    writeln(here.id, " ", c.x);
  }
  writeln(here.id, " ", c.x);

  delete c;
}
