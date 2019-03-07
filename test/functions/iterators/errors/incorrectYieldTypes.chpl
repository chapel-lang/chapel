
proc myYieldType() type return int;

iter IT(): myYieldType() {
  if numLocales > 1 then
    yield Locales;
  else
    yield LocaleSpace;
}

proc main {
  for idx in IT() do
    writeln(idx);
}
