inline proc writestuff(newLine=true) {
  writeln((here, here._instance:LocaleModel?, here._instance:NumaDomain?));
  if newLine then writeln();
}

writestuff();

coforall i in 0..#here.getChildCount() do
  on here.getChild(i) do writestuff(false);

writeln("==========");
writestuff();

coforall subloc in (here._value:LocaleModel).getChildren() do
  on subloc do writestuff(false);

writeln("==========");
writestuff();

coforall subloc in (here._value:LocaleModel).getChildren() do
  on subloc do writestuff(false);

writeln("==========");
writestuff();

config const dummy = true; // to break up the coforall+on optimization
coforall subloc in (here._value:LocaleModel).getChildren() {
  if dummy then on subloc do writestuff(false);
  else writestuff(false);
}

writeln("==========");
writestuff();

