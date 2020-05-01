extern proc foo(s: c_string);
extern proc bar(i: int): c_string;

foo("hello world");
var s = bar(12);
writeln(createStringWithNewBuffer(s));
