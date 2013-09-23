inline proc writestuff(newLine=true) {
  writeln((here, here:LocaleModel, here:NumaDomain));
  if newLine then writeln();
}

writestuff();

coforall i in 0..#(here:LocaleModel).numSublocales do
  on (here:LocaleModel).getChild(i) do writestuff(false);

writeln();
writestuff();

coforall subloc in (here:LocaleModel).getChildren() do
  on subloc do writestuff(false);

writeln();
writestuff();

coforall subloc in (here:LocaleModel).getChildren() do
  on subloc do writestuff(false);

writeln();
writestuff();

config const dummy = true; // to break up the coforall+on optimization
coforall subloc in (here:LocaleModel).getChildren() {
  if dummy then on subloc do writestuff(false);
  else writestuff(false);
}

writeln();
writestuff();

