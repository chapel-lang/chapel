_extern def foo(s: string);
_extern def bar(i: int): string;

foo("hello world");
var s = bar(12);
writeln(s);
