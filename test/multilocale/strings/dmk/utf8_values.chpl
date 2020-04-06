var mystring1 = "hello";

on Locales[numLocales-1] {
  for (i, c) in zip(0.., mystring1.bytes()) {
    const str = codepointToString(c);
    write(c);
    var c2 = mystring1.byte(i);
    if c2 != c then
      write("#", c2);
    on Locales[0] {
      write("\t", str);
      for (j, bval) in zip(0.., str.bytes()) {
        write(" ", bval);
        var bval2 = str.byte(j);
        if bval2 != bval then
          write("#", bval2);
      }
    }
    writeln();
  }
}

var mystring2 = "événement";

on Locales[numLocales-1] {
  for (i, c) in zip(0.., mystring2.codepoints()) {
    const str = codepointToString(c);
    write(c);
    var c2 = mystring2.codepoint(i);
    if c2 != c then
      write("#", c2);
    on Locales[0] {
      write("\t", str);
      for (j, bval) in zip(0.., str.bytes()) {
        write(" ", bval);
        var bval2 = str.byte(j);
        if bval2 != bval then
          write("#", bval2);
      }
    }
    writeln();
  }
}
