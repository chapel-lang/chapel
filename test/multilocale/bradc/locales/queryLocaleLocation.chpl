// make sure that each locale is on its actual locale
for i in 0..numLocales-1 do
  writeln("Locale # ", i, " is on locale: ", rootLocale.getLocales()(i).locale.id);
writeln();

// The above has been known to be cheated in the compiler, so the
// following are attempts to look beyond this cheat:

// since .id is now just a const method, this generates a warning
// correctly and should generate 0 in all cases
for i in 0..numLocales-1 do
  writeln("Locale # ", i, " is on locale: ", rootLocale.getLocales()(i).id.locale.id);
writeln();

// This is a little suspect since it relies on internal knowledge;
// however, it's a nice sanity check that we aren't pulling any
// tricks within the compiler:
for i in 0..numLocales-1 do
  writeln("Locale # ", i, " is on locale: ", rootLocale.getLocales()(i).chpl_id.locale.id);
writeln();
