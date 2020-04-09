proc main {
  var x: int = 17;
  writeln(here.id, " x=", x);
  x += 1;
  on Locales(1) {
    writeln(here.id, " x=", x);
    x += 1;
    on Locales(0) {
      writeln(here.id, " x=", x);
      x += 1;
    }
    writeln(here.id, " x=", x);
  }
  writeln(here.id, " x=", x);
}
