proc int.double() {
  return this*2;
}

on Locales[numLocales-1] {
  var x : [1..4] int = 1;
  writeln("x = ", x.double());
}
