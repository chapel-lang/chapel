iter foo() {
  for loc in Locales do
    on loc do
      yield here.id;
}

for f in foo() do
  writeln("Got ", f, " on locale ", here.id);
