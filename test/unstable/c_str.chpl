use CTypes only c_str;
var s = "my string";
var b = b"my bytes";

var cs = s.c_str();
var bs = b.c_str();

writeln(string.createBorrowingBuffer(cs));
writeln(string.createBorrowingBuffer(bs));
