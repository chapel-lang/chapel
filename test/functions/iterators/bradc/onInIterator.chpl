iter foo() {
  on Locales[numLocales-1] do
    writeln("On locale ", here.id);
  yield 3;
}

for i in foo() do writeln(i);
