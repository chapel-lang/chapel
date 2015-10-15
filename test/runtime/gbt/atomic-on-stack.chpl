proc main() {
  var x: atomic int;
  on Locales[numLocales-1] do
    x.add(1);
  writeln(x.read());
}
