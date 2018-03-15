pragma "safe"
module arrref {

proc badReturnRefLocalArrayElement() ref {
  var A:[1..10] int;

  return A[1];
  // A and its elements are destroyed here
}

proc test() {
  ref v1 = badReturnRefLocalArrayElement();
  writeln(v1);
}

test();
}
