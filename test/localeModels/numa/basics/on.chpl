inline proc writestuff(newLine=true) {
  writeln((here, here._instance:LocaleModel?, here._instance:NumaDomain?));
  if newLine then writeln();
}

writestuff();

writeln(Locales[numLocales-1]);
on (Locales[numLocales-1]) {
  writestuff();
}

writestuff();

const subloc = Locales[numLocales-1].getChildCount()-1;
writeln(Locales[numLocales-1].getChild(subloc));
on Locales[numLocales-1].getChild(subloc) {
  writestuff();
}

writestuff();

writeln(here.getChild(subloc));
on here.getChild(subloc) {
  writestuff();
}

writestuff();

