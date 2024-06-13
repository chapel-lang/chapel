{
  on Locales.last var A: [1..10] int;
  writeln(A);
  writeln(A.locale == Locales.last);

  on Locales.last var B = A;
  writeln(B);
  writeln(B.locale == Locales.last);

  on Locales.last var C = A + 1;
  writeln(C);
  writeln(C.locale == Locales.last);

  /* Write to them to ensure they're mutable. */
  A[1] = 42;
  writeln(A);
  B[1] = 43;
  writeln(B);
  C[1] = 44;
  writeln(C);
}
{
  on Locales.last const A: [1..10] int;
  writeln(A);
  writeln(A.locale == Locales.last);

  on Locales.last const B = A;
  writeln(B);
  writeln(B.locale == Locales.last);

  on Locales.last const C = A + 1;
  writeln(C);
  writeln(C.locale == Locales.last);

  /* Const-ness errors tested separately to avoid compile-time errors. */
}
