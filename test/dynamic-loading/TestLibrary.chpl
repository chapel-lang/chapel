export proc foo() {
  for loc in Locales do on loc do writeln('Hello: ', here.id);
}
