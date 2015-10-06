sync {

  on Locales[numLocales-1] {

    const mystr = "hello" + here.id;

    begin with (ref mystr) on Locales[0] {
      writeln(mystr);
    }
  }
}

