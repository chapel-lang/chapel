writeln("On initial locale: ", here.id);
on rootLocale.getLocale(1) {
  writeln("On locale ", here.id);
}
writeln("Back on initial locale: ", here.id);
