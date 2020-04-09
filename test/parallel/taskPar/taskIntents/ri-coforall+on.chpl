
var total : int;

coforall loc in Locales with (+ reduce total) do
  on loc do
    total += loc.id;

writeln("numLocales ", numLocales, "  total ", total);

/* The compiler used to be sensitive to 'do...' vs. '{...}' syntax.
   So below we are testing all the variations. */

coforall loc in Locales with (+ reduce total) do
  on loc {
    total += loc.id;
  }

writeln("do    {}    numLocales ", numLocales, "  total ", total);

coforall loc in Locales with (+ reduce total) do
  on loc do {
    total += loc.id;
  }

writeln("do    do{}  numLocales ", numLocales, "  total ", total);


/// coforall {} ///

coforall loc in Locales with (+ reduce total) {
  on loc do
    total += loc.id;
}

writeln("{}    do    numLocales ", numLocales, "  total ", total);

coforall loc in Locales with (+ reduce total) {
  on loc {
    total += loc.id;
  }
}

writeln("{}    {}    numLocales ", numLocales, "  total ", total);

coforall loc in Locales with (+ reduce total) {
  on loc do {
    total += loc.id;
  }
}

writeln("{}    do{}  numLocales ", numLocales, "  total ", total);


/// coforall do{} ///

coforall loc in Locales with (+ reduce total) do {
  on loc do
    total += loc.id;
}

writeln("do{}  do    numLocales ", numLocales, "  total ", total);

coforall loc in Locales with (+ reduce total) do {
  on loc {
    total += loc.id;
  }
}

writeln("do{}  {}    numLocales ", numLocales, "  total ", total);

coforall loc in Locales with (+ reduce total) do {
  on loc do {
    total += loc.id;
  }
}

writeln("do{}  do{}  numLocales ", numLocales, "  total ", total);
