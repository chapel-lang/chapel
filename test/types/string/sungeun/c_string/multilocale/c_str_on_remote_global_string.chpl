use CTypes;
var s = "0123456789";
on Locales[numLocales-1] {
  writeln(string.createCopyingBuffer(s.c_str()));
}
