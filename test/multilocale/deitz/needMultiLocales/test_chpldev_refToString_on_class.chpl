class C {
  var x: int;
}

var c: unmanaged C?;

on Locales(1) {
  c = new unmanaged C();
}

var cc = c:unmanaged C;
writeln(chpldev_refToString(cc));
writeln(chpldev_refToString(cc.x));

delete c;
