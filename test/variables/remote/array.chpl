{
  on Locales[1] var A: [1..10] int;
  writeln(A);
  writeln(A.locale == Locales[1]);

  on Locales[1] var B = A;
  writeln(B);
  writeln(B.locale == Locales[1]);

  on Locales[1] var C = A + 1;
  writeln(C);
  writeln(C.locale == Locales[1]);

  /* Write to them to ensure they're mutable. */
  A[1] = 42;
  writeln(A);
  B[1] = 43;
  writeln(B);
  C[1] = 44;
  writeln(C);
}
{
  on Locales[1] const A: [1..10] int;
  writeln(A);
  writeln(A.locale == Locales[1]);

  on Locales[1] const B = A;
  writeln(B);
  writeln(B.locale == Locales[1]);

  on Locales[1] const C = A + 1;
  writeln(C);
  writeln(C.locale == Locales[1]);

  /* Const-ness errors tested separately to avoid compile-time errors. */
}
