proc dit() {
  writeln(here);
  writeln("rootLocale is on ", rootLocale.locale);
  writeln("Locales is on ", Locales.locale);
  for loc in Locales do writeln(" ", loc.locale);
  writeln();
}

for loc in Locales do on loc do dit();



