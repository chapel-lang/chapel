extern proc foo(s: string);
extern proc bar(i: int): string;

foo("hello world");
var s = bar(12);
writeln(s);
