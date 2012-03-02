iter foo() {
  on Locales[numLocales-1] do
    yield 3;
}

for i in foo() do writeln(i);
