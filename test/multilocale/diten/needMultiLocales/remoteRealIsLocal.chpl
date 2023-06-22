
class C {
  var D: real;
}
proc main {
  var D: real;
  writeln(D);
  writeln(D.locale.id);
  on Locales(1) {
    var cObj = new owned C(1.10);
    var c = cObj.borrow();
    writeln(D.locale.id);
    D = c.D;
    writeln(c.D.locale.id);
    writeln(D.locale.id);
  }
  writeln(D.locale.id);
  writeln(D);
}
