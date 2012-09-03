config var size = 5;
proc main {
  var D: domain(locale);
  for loc in rootLocale.getLocales() {
    D += loc;
  }
  // A is an associative array indexed by locales of arithmetic arrays of reals
  // A lives entirely on Locales(0).
  var A: [D] [1..size] real;
  sync {
    [loc in D] begin on loc {
      [i in 1..size]
        A(loc)(i) = loc.id + i / 10.0;
    }
  }
  for loc in rootLocale.getLocales() {
    writeln(A(loc));
  }
}
