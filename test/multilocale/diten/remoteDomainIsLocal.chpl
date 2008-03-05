
class C {
  var D: domain(1);
}
def main {
  var D: domain(1);
  writeln(D);
  writeln(D.locale);
  on Locales(1) {
    var c = new C([1..10]);
    writeln(D.locale);
    D = c.D;
    writeln(c.D.locale);
    writeln(D.locale);
  }
  writeln(D.locale);
  writeln(D);
}
