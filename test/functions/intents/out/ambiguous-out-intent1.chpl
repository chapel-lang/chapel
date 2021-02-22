proc foo(out a: int) {
  writeln("foo(out a: int)");
}

proc foo(out a: uint) {
  writeln("foo(out a: uint)");
}

var oneInt:int = 1;
foo(oneInt);
var oneUint:uint = 1;
foo(oneUint);
