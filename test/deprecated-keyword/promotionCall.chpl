@deprecated()
proc foo(x: int) {
  writeln(x);
}

proc main() {
  var arr: [1..3] int;
  arr[2] = 7;
  foo(arr); // This should trigger the deprecation warning
}
