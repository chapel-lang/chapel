on Locales[0] {

  const mystr = "hello" + here.id;

  on Locales[numLocales-1] {
    write(mystr);
    writeln();
  }
}

