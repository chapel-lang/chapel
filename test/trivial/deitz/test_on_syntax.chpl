var x: int = 2;

on rootLocale.getLocales()(0) {
  var y: int = 3;
  writeln(y);
}

writeln(x);

on rootLocale.getLocales()(0) do writeln("bye");
