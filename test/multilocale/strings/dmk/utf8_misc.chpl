var mystring1: string = "événement";
var cplen1 = mystring1.numCodepoints;

on Locales[numLocales-1] {
  writeln("Byte length = ", mystring1.length);
  writeln("Codepoint length = ", cplen1);

  for i in 1..cplen1 {
    writeln("Codepoint index ", i, " = '", mystring1[i: codepointIndex], "'");
  }
}
