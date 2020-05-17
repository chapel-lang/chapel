class C {
  var x: int;
}

proc main() {

  var A:[1..1] owned C = [new owned C(1)];

  var B = A; // not copy elided

  A;
}
