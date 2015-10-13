sync {

  on Locales[numLocales-1] {

    const mystr = "hello" + here.id;

    begin with (in mystr) on Locales[0] {
      write(mystr);
      writeln();
    }
  }
}

