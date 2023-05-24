{
  var cs: c_string;
  on Locales[numLocales-1] {
    cs = c_ptrToConst_helper("0123456789"):c_string; // this should result in a runtime error
  }
  writeln(cs:string);
}
