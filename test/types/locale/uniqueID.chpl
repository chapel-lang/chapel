use Set;
writeln(here.uid);

var idset = new set(int);
for loc in Locales do idset.add(loc.uid);

writeln(idset.size == numLocales);

writeln(here.uid == Locales[0].uid);
writeln(here.uid != Locales[1].uid);
writeln(here.uid == Locales[1].uid);
