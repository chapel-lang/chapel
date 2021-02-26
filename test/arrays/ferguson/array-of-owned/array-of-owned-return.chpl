class C { var x: int; }

proc makeArrayOwned() {
  var A: [1..3] owned C? = for i in 1..3 do new owned C?(i);
  var B: [1..3] owned C = A:owned C;
  return B;
}

proc main() {
  var arr = makeArrayOwned();
  writeln(arr);
}
