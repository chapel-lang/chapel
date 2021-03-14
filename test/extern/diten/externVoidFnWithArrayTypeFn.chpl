require "testVoidExternFns.h";

proc returnVoidType() type return void;

extern proc voidWithArray(A: [] int, size: int): returnVoidType();

var A: [1..3] int = 1..3;

voidWithArray(A, 3);
