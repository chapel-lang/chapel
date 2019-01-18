proc main() {
  on Locales[numLocales-1] {
    var x: atomic int;
    on Locales[0] do
      x.add(1);
    writeln(x.read());
  }
}
