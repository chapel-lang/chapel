var mystring1 = "événement";

on Locales[numLocales-1] {
  for (i, c) in zip(1.., mystring1.codePoints()) {
    const str = codePointToString(c);
    write(c);
    var c2 = mystring1.codePoint(i);
    if c2 != c then
      write("#", c2);
    on Locales[0] {
      write("\t", str);
      for (j, bval) in zip(1.., str.bytes()) {
        write(" ", bval);
        var bval2 = str.byte(j);
        if bval2 != bval then
          write("#", bval2);
      }
    }
    writeln();
  }
}
