proc foo() {
  const x: int;

  on Locales(numLocales-1) do x = 2;

  writeln("x is: ", x);
}

foo();
