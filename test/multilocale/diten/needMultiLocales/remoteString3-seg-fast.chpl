proc main {
  var s = "first string";
  on Locales(1) {
    s = "second string";
  }
  writeln(s.locale);
}
