_extern proc foo(s: string);
_extern proc bar(i: int): string;

foo("hello world");
var s = bar(12);
writeln(s);
