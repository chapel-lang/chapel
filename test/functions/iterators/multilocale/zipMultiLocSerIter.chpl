iter foo() {
  for loc in Locales do
    on loc do
      yield here.id;
}

iter bar() {
  for locid in LocaleSpace by -1 do
    on Locales[locid] do
      yield here.id;
}

for (i,j) in zip(foo(),bar()) do
  writeln("Got ", (i,j), " on locale ", here.id);

for (i,j) in zip(bar(),foo()) do
  writeln("Got ", (i,j), " on locale ", here.id);
