use CTypes;
config type T = string;
var helloBytes: [1..6] uint(8) = [104, 101, 108, 108, 111, 0]:uint(8);
{
  var s: T = "hello";
  var s_ = T.createBorrowingBuffer(s);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = T.createBorrowingBuffer(c_ptrTo(b), length=5);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = T.createBorrowingBuffer(c_ptrToConst(b), length=5);
  writeln(s_);
}
{
  var b: [1..6] uint(8) = helloBytes;
  var s_ = T.createBorrowingBuffer(c_ptrTo(b), length=5, size=6);
  writeln(s_);
}
