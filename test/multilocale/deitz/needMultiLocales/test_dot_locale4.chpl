class C { }
on rootLocale.getLocales()(0) {
  var x: int;
  var c: C;
  on rootLocale.getLocales()(1) {
    on rootLocale.getLocales()(2) {
      c = new C();
    }
    writeln(x.locale.id);
    writeln(c.locale.id);
  }
}
