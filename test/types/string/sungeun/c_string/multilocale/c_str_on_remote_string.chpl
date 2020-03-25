{
  var s : string;
  for i in 0..9 do
    s += i:string;

  on Locales[numLocales-1] {
    var cs = s.c_str();
    var s2 = createStringWithNewBuffer(cs);
    writeln(s2);
  }
}
