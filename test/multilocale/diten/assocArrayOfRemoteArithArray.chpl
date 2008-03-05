config var size = 5;
def main {
  var D: domain(locale);
  [loc in LocalesDomain] D += Locales(loc);
  // A is an associative array indexed by locales of arithmetic arrays of reals
  // A lives entirely on Locales(0).
  var A: [D] [1..size] real;
  sync {
    [loc in D] begin on loc {
      [i in 1..size]
        A(loc)(i) = i + loc.id / 10.0;
    }
  }
  writeln(A);
}
