use Set;
writeln(here.uid);

var idset = new set(localeUID);
for loc in Locales do idset.add(loc.uid);

writeln(idset.size == numLocales);
