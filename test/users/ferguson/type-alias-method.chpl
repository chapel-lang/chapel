use CTypes;
type my_type = c_ptrConst(c_char);

proc my_type.doit() {
  writeln(string.createBorrowingBuffer(this));
}

var str = "bla".c_str();
str.doit();

