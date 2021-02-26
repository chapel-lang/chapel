class C {
  var x: int;
}

proc main() {

  var A:[1..1] owned C = [new owned C(1)];

  var B:[1..1] owned C = A; // not copy elided

  A;
}
