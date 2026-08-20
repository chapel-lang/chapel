proc foo(a): [] {
  return a;
}
proc bar(a, b): ([], []) {
  return (a, b);
}
proc baz(const a, const b, const c): ([], ([], [])) {
  return (a, (b, c));
}

var A = [1, 2, 3];
writeln(foo(A));
var B = [4, 5, 6];
writeln(bar(A, B));
var C = [7.0, 8.0, 9.0, 10.0];
writeln(baz(A, B, C));
