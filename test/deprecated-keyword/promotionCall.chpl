@deprecated()
proc foo(x: int) {
  writeln(x);
}

proc main() {
  var arr: [1..3] int;
  arr[2] = 7;
  foo(arr); // This should trigger the deprecation warning
  var arr2: [-4..-2] int = 3;
  foo(arr2); // As should this
}
