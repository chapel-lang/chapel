proc computeValue() {
  writeln("Computing value on last? ", here == Locales.last);
  return 42;
}

on Locales.last var a, b = computeValue();
writeln(a.locale == Locales.last);
writeln(b.locale == Locales.last);

/*

Disabled due to https://github.com/chapel-lang/chapel/issues/25748

    proc computeType(x: int, y: int) type {
      writeln("Computing type from x=", x, " and y=", y, " on last? ", here == Locales.last);
      return [x..y] int;
    }

    on Locales.last var A, B: computeType(1, 16);
    writeln(A.locale == Locales.last);
    writeln(B.locale == Locales.last);

*/

proc computeSimpleType(x: int, y: int) type {
  writeln("Computing type from x=", x, " and y=", y, " on last? ", here == Locales.last);
  return int;
}

on Locales.last var x, y: computeSimpleType(1, 16);
writeln(x.locale == Locales.last);
writeln(y.locale == Locales.last);
