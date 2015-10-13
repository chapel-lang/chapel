sync {

  on Locales[0] {

    const mystr = "hello" + here.id;

    begin with (in mystr) on Locales[numLocales-1] {
      write(mystr);
      writeln();
    }
  }
}

