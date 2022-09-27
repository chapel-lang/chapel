proc foo(x: real, y: real, start: int, end: int, stride: int) {
  writeln("foo(real real int int int)");
}
proc foo(x: string, y: string, start: int, end: int, stride: int) {
  writeln("foo(string string int int int)");
}

var u:uint(8) = 1;
foo(1.2, 3.4, u, u, u);
// seems like it should call the first rather than saying it doesn't know what to do since I've relied on > 2 coercions.
