proc foo(x) {
  const s: string = "hi";
  on Locales[numLocales-1] {
    writeln(s+x:string);
  }
}

foo("s");
