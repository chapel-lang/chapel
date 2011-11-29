proc main {
  var x: int = 17;
  writeln(here.id, " x=", x);
  x += 1;
  writeln(here.id, " x.locale.id=", x.locale.id);
  on Locales(1) {
    writeln(here.id, " x=", x);
    x += 1;
    writeln(here.id, " x.locale.id=", x.locale.id);
  }
  writeln(here.id, " x=", x);
}
