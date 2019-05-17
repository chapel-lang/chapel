require "testVoidExternFns.h";

type voidAlias = void;

extern proc voidWithArray(A: [] int, size: int): voidAlias;

var A: [1..3] int = 1..3;

voidWithArray(A, 3);
