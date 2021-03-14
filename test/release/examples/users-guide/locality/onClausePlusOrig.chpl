for loc in Locales {
  writeln("Started on locale #", here.id);
  on loc do
    writeln("Hello from locale #", here.id);
  writeln("Back on    locale #", here.id, "\n");
}
