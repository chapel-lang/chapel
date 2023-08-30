use CTypes;
var cs: c_ptrConst(c_char);
on Locales[numLocales-1] {
  cs = "0123456789".c_str(); // this should result in a runtime error
}
writeln(string.createBorrowingBuffer(cs));
