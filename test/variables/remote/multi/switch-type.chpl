type T1 = [1..10] int;
type T2 = [1..10] real;

on Locales.last var A, B: T1,
                    C, D: T2;
writeln(A);
writeln(A.locale == Locales.last);
writeln(B);
writeln(B.locale == Locales.last);
writeln(C);
writeln(C.locale == Locales.last);
writeln(D);
writeln(D.locale == Locales.last);
