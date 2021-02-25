record Foo {
  var x: int;
}
operator Foo.==(lhs: Foo, rhs: Foo) {
  return lhs.x == rhs.x;
}
proc main() {
  var arr1: [0..3] Foo;
  var arr2: [0..3] Foo;
  writeln(& reduce (arr1 == arr2));
}
