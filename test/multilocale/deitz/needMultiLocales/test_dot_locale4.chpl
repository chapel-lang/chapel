class C { }
on Locales(0) {
  var x: int;
  var c: unmanaged C?;
  on Locales(1) {
    on Locales(2) {
      c = new unmanaged C();
    }
    writeln(x.locale.id);
    writeln(c.locale.id);
  }
  delete c;
}
