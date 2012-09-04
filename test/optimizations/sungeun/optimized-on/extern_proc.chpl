extern proc foo(x: int);

on rootLocale.getLocale(numLocales-1) {
  for l in 1..numLocales-1 by -1 do
    on rootLocale.getLocale(l) {
      foo(l);
    }
}

