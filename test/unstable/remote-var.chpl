{
  on Locales.last var x = 42;
  writeln(x);
  writeln(x.locale == Locales.last);

  on Locales.last var y: int;
  writeln(y);
  writeln(y.locale == Locales.last);

  on Locales.last var z: int = 42;
  writeln(z);
  writeln(z.locale == Locales.last);

  /* Write to them to ensure they're mutable. */
  x = 17;
  y = 17;
  z = 17;
  writeln(x);
  writeln(y);
  writeln(z);
}
{
  on Locales.last const x = 42;
  writeln(x);
  writeln(x.locale == Locales.last);

  on Locales.last const y: int;
  writeln(y);
  writeln(y.locale == Locales.last);

  on Locales.last const z: int = 42;
  writeln(z);
  writeln(z.locale == Locales.last);

  /* Const-ness errors tested separately to avoid compile-time errors. */
}
