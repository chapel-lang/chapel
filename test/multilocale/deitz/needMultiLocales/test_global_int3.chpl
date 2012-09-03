var x = 1;
writeln("(x, x.locale.id) = ", (x, x.locale.id));
on rootLocale.getLocales()(1) {
  x = x + 1;
  writeln("(x, x.locale.id) = ", (x, x.locale.id));
  var y = x + 1;
  writeln("(y, y.locale.id) = ", (y, y.locale.id));
}
x = x + 1;
writeln("(x, x.locale.id) = ", (x, x.locale.id));
