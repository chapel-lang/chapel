
class C {
  var D: real;
}
proc main {
  var D: real;
  writeln(D);
  writeln(D.locale.id);
  on Locales(1) {
    var D: real;
    writeln(D.locale.id);
    on Locales(0) {
      writeln(D.locale.id);
    }
  }
  writeln(D.locale.id);
  writeln(D);
}
