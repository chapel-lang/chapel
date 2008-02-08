def main {
  var x: [1..3] int = (4, 5, 6);
  writeln((localeID(), x));
  x = x + 1;
  writeln((localeID(), x));
  on Locales(1) {
    writeln((localeID(), x));
    x = x + 1;
    writeln((localeID(), x));
  }
  writeln((localeID(), x));
}
