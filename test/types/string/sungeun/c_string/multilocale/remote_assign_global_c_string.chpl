use CTypes;
var cs: c_ptrConst(c_char);
on Locales[numLocales-1] {
  cs = c_ptrToConst_helper("0123456789"); // this should result in a runtime error
}
writeln(string.createBorrowingBuffer(cs));
