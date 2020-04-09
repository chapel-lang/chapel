extern proc foo(x: int);

on Locales(numLocales-1) {
  for l in 1..numLocales-1 by -1 do
    on Locales(l) {
      foo(l);
    }
}

