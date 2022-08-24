
class C {
  var D: domain(1);
}
proc main {
  var D: domain(1);
  writeln(D);
  writeln(D.locale.id);
  on Locales(1) {
    var c = (new owned C({1..10})).borrow();
    writeln(D.locale.id);
    D = c.D;
    writeln(c.D.locale.id);
    writeln(D.locale.id);
  }
  writeln(D.locale.id);
  writeln(D);
}
