// TODO: Should be able to assign to most general type.
var f: proc(_: int): int = proc(i: int) { return i; };
var g = proc(j: int) { return j*j; };
writeln(f(10));
f = g;
writeln(f(10));
