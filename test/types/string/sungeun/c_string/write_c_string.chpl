private use CTypes;
private use ChapelSysCTypes;
{
  var s = "0123456789";
  on Locales[numLocales-1] {
    writeln(s:c_ptrConst(c_char):c_string);
  }
}
