inline proc writestuff(newLine=true) {
  writeln((here, here:LocaleModel, here:NumaDomain));
  if newLine then writeln();
}

writestuff();

writeln(Locales[numLocales-1]);
on (Locales[numLocales-1]) {
  writestuff();
}

writestuff();

const subloc = (Locales[numLocales-1]:LocaleModel).numSublocales-1;
writeln((Locales[numLocales-1]:LocaleModel).getChild(subloc));
on (Locales[numLocales-1]:LocaleModel).getChild(subloc) {
  writestuff();
}

writestuff();

writeln((here:LocaleModel).getChild(subloc));
on (here:LocaleModel).getChild(subloc) {
  writestuff();
}

writestuff();

