{
  var cs: c_string;
  on Locales[numLocales-1] {
    cs = "0123456789".c_str(); // this should result in a runtime error
  }
  writeln(cs:string);
}
