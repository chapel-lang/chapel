{
  var cs: c_string;
  on Locales[numLocales-1] {
    cs = "0123456789":c_ptrConst(c_char):c_string; // this should result in a runtime error
  }
  writeln(cs:string);
}
