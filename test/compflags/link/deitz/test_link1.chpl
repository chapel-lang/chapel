use CTypes;
extern proc foo(s: c_ptrConst(c_char));
extern proc bar(i: int): c_ptrConst(c_char);

foo("hello world");
var s = bar(12);
writeln(string.createCopyingBuffer(s));
