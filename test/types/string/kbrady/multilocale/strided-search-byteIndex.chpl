var x = "a1b2c3d4e5";

var y = "abcd abcd";

on Locales[numLocales-1] {
  writeln(x.find("abcde", 1:byteIndex.. by 2));
  writeln(x.find("12345", 1:byteIndex.. by 2));
  writeln(x.find("12345", 2:byteIndex.. by 2));
  writeln(y.find("dcba", 1:byteIndex.. by -1));
}
