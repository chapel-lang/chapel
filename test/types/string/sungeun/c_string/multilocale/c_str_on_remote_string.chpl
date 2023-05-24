{
  use CTypes;
  var s : string;
  for i in 0..9 do
    s += i:string;

  on Locales[numLocales-1] {
    var cs = c_ptrToConst_helper(s):c_string;
    var s2 = string.createCopyingBuffer(cs);
    writeln(s2);
  }
}
