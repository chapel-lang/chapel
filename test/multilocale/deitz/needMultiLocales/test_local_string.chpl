proc main {
  var x: string = "hello world";
  writeln(here.id, " x=", x);
  x = x + 1;
  on Locales(1) {
    writeln(here.id, " x=", x);
    x = x + 1;
  }
  writeln(here.id, " x=", x);
}
