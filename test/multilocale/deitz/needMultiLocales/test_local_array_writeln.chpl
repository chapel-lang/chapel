proc main {
  var x: [1..3] int = (4, 5, 6);
  writeln((here.id, x));
  x = x + 1;
  writeln((here.id, x));
  on Locales(1) {
    writeln((here.id, x));
    x = x + 1;
    writeln((here.id, x));
  }
  writeln((here.id, x));
}
