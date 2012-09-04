proc main {
  var x: string = "hello world";
  writeln(here.id, " x=", x);
  x = x + 1;
  on rootLocale.getLocale(1) {
    writeln(here.id, " x=", x);
    x = x + 1;
  }
  writeln(here.id, " x=", x);
}
