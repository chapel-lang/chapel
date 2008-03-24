
class C {
  var D: real;
}
def main {
  var D: real;
  writeln(D);
  writeln(D.locale);
  on Locales(1) {
    var D: real;
    writeln(D.locale);
    on Locales(0) {
      writeln(D.locale);
    }
  }
  writeln(D.locale);
  writeln(D);
}
