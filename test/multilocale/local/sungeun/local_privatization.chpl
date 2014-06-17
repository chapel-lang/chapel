config const n = numLocales;

forall loc in Locales {
  local {
    const D = {1..10};
    var A: [D] int;
    A = loc.id;
  }
}

writeln("All done!");
