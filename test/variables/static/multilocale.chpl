proc add(arg) {
  @functionStatic(sharingKind.computePerLocale)
  ref counter = 0;

  counter += arg;
  return counter;
}

for loc in Locales do on loc {
  add(1);
  add(2);
  add(3);
  writeln("Locale ", here.id, " counter: ", add(0));
}
