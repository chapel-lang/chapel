var x: int = 2;

on Locale(0) {
  var y: int = 3;
  writeln(y);
}

writeln(x);

on Locale(0) do writeln("bye");
