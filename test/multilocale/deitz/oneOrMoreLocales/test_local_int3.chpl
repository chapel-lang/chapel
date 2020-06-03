proc main {
  var x: int = 17;
  writeln(here.id, " x=", x);
  x = x+1;
  for i in 0..numLocales-1 {
    on Locales(i) {
      writeln(here.id, " x=", x);
      x = x+1;
    }
  }
  writeln(here.id, " x=", x);
}

