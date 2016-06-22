var x: int = 2;

on Locales(0) {
  var y: int = 3;
  writeln(y);
}

writeln(x);

on Locales(0) do writeln("bye");
