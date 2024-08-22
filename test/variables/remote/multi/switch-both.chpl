on Locales.last var A, B: [1..10] int = 2, C: [1..10] int, d, e = 2;
writeln(A);
writeln(B);
writeln(C);
writeln(d);
writeln(e);
writeln(A.locale == Locales.last &&
        B.locale == Locales.last &&
        C.locale == Locales.last &&
        d.locale == Locales.last &&
        e.locale == Locales.last);
