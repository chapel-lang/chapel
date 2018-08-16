class C {
  var x: int;
}

var c: unmanaged C;

on Locales(1) {
  c = new unmanaged C();
}

writeln(chpldev_refToString(c));
writeln(chpldev_refToString(c.x));

delete c;
