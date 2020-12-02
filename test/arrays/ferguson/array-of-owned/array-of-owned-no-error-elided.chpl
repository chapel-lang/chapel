class C {
  var x: int;
}

proc test1() {
  var A:[1..1] owned C = [new owned C(1)];

  var B:[1..1] owned C = A; // copy elided
}
test1();

proc test2() {
  var A:[1..1] owned C = [new owned C(1)];

  var B = A; // copy elided
}
test2();
