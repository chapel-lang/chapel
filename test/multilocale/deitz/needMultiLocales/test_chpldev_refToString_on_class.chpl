class C {
  var x: int;
}

var c: C;

on Locales(1) {
  c = new C();
}

writeln(chpldev_refToString(c));
writeln(chpldev_refToString(c.x));
