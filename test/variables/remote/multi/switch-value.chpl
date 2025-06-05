proc computeValue(x: int) {
  writeln("Computing value on last? ", here == Locales.last);
  return x;
}

on Locales.last var a, b = computeValue(42),
                    c, d = computeValue(17);
writeln((a, b, c, d));
