use CTypes;
type my_type = c_ptrConst(c_uchar);

proc my_type.doit() {
  writeln(string.createBorrowingBuffer(this));
}

var str = c_ptrToConst_helper("bla");
str.doit();

