proc main {
  var x: int = 17;
  var y: int = 23;
  writeln(here.id, " x=", x);
  writeln(here.id, " y=", y);
  x = x + 1;
  y = y + 1;
  on Locales(1) {
    writeln(here.id, " x=", x);
    writeln(here.id, " y=", y);
    x = x + 1;
    y = y + 1;
  }
  writeln(here.id, " x=", x);
  writeln(here.id, " y=", y);
}
