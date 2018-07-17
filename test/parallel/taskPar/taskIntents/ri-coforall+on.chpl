
var total : int;

coforall loc in Locales with (+ reduce total) do
  on loc do
    total += loc.id;

writeln("numLocales ", numLocales, "  total ", total);
