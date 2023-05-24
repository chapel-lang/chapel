use CTypes;
extern proc foo(s: c_ptrConst(c_uchar));
extern proc bar(i: int): c_ptrConst(c_uchar);

foo(c_ptrToConst_helper("hello world"));
var s = bar(12);
writeln(string.createCopyingBuffer(s));
