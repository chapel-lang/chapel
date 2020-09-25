require "testVoidExternFns.h";

config const n = 2;
var A: [1..n] int = [1, 2];

extern proc voidWithArray(A: [] int, n: int): void;

var res = voidWithArray(A, n);
writeln(res.type:string);
