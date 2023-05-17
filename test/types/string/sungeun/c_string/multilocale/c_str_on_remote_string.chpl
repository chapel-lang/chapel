{
  var s : string;
  for i in 0..9 do
    s += i:string;

  on Locales[numLocales-1] {
    var cs = s:c_ptrConst(c_char):c_string;
    var s2 = string.createCopyingBuffer(cs);
    writeln(s2);
  }
}
