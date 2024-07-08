iter myIter() {
  yield 1;
  yield 2;
  yield 3;
}

{
  on Locales.last var A: [1..10] int;
  writeln(A);
  writeln(A.locale == Locales.last);
  on A do writeln(here == Locales.last);

  on Locales.last var B = A;
  writeln(B);
  writeln(B.locale == Locales.last);
  on B do writeln(here == Locales.last);

  on Locales.last var C = A + 1;
  writeln(C);
  writeln(C.locale == Locales.last);
  on C do writeln(here == Locales.last);

  on Locales.last var D = myIter();
  writeln(D);
  writeln(D.locale == Locales.last);
  on D do writeln(here == Locales.last);

  on Locales.last var E = for i in 1..10 do i * i;
  writeln(E);
  writeln(E.locale == Locales.last);
  on E do writeln(here == Locales.last);

  on Locales.last var F = for i in 1..10 do myIter();
  writeln(F);
  writeln(F.locale == Locales.last);
  on F do writeln(here == Locales.last);

  /* Write to them to ensure they're mutable. */
  A[1] = 42;
  writeln(A);
  B[1] = 43;
  writeln(B);
  C[1] = 44;
  writeln(C);
  D[1] = 45;
  writeln(D);
  E[1] = 46;
  writeln(E);
  F[1] = 47;
  writeln(F);
}
{
  on Locales.last const A: [1..10] int;
  writeln(A);
  writeln(A.locale == Locales.last);
  on A do writeln(here == Locales.last);

  on Locales.last const B = A;
  writeln(B);
  writeln(B.locale == Locales.last);
  on B do writeln(here == Locales.last);

  on Locales.last const C = A + 1;
  writeln(C);
  writeln(C.locale == Locales.last);
  on C do writeln(here == Locales.last);

  on Locales.last const D = myIter();
  writeln(D);
  writeln(D.locale == Locales.last);
  on D do writeln(here == Locales.last);

  /* Const-ness errors tested separately to avoid compile-time errors. */
}
