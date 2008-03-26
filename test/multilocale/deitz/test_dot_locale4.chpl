class C { }
on Locales(0) {
  var x: int;
  var c: C;
  on Locales(1) {
    on Locales(2) {
      c = new C();
    }
    writeln(x.locale.id);
    writeln(c.locale.id);
  }
}
