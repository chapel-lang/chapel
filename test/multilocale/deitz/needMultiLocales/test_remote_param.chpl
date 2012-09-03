proc main {
  param p = 2;
  on rootLocale.getLocales()(1) {
    writeln(p);
  }
}
