var x = 1;
writeln("(x, x.locale.uid) = ", (x, x.locale.uid));
on Locales(1) {
  x = x + 1;
  writeln("(x, x.locale.uid) = ", (x, x.locale.uid));
  var y = x + 1;
  writeln("(y, y.locale.uid) = ", (y, y.locale.uid));
}
x = x + 1;
writeln("(x, x.locale.uid) = ", (x, x.locale.uid));
