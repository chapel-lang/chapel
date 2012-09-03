proc main {
  var s = "first string";
  on rootLocale.getLocales()(1) {
    s = "second string";
  }
  writeln(s.locale);
}
