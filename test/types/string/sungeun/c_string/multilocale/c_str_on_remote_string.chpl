{
  var s = "0123456789";
  on Locales[numLocales-1] {
    var cs = s.c_str();
    var s2 = cs: string;
    writeln(s2);
  }
}
