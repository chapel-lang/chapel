require "testVoidExternFns.h";

extern proc voidNoArray(n: int): void;

var res = voidNoArray(4);
writeln(res.type:string);
