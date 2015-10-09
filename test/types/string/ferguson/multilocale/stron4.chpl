on Locales[numLocales-1] {

  const mystr = "hello" + here.id;

  on Locales[0] {
    write(mystr);
    writeln();
  }
}

