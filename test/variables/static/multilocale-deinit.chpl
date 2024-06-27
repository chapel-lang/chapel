record R {
  var x: int;

  proc deinit() {
    writeln("deinit of record with value ", x);
  }

  proc ref increment() do x += 1;
}

proc f() {
  @functionStatic(sharingKind.computePerLocale)
  ref myVar = new R(0);
  myVar.increment();
}

for loc in Locales do on loc {
  for 1..3 do f();
}
