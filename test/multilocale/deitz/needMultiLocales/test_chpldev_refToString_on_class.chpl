class C {
  var x: int;
}

var c: C;

on rootLocale.getLocales()(1) {
  c = new C();
}

writeln(chpldev_refToString(c));
writeln(chpldev_refToString(c.x));
