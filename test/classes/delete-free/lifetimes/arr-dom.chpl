pragma "safe"
module arrdom {
pragma "no copy return"
proc badReturnBorrowedArrayDom() {
  var A:[1..10] int;

  return A.domain;
  // A and its elements are destroyed here
}

proc badReturnBorrowedArrayDomDist() {
  var A:[1..10] int;

  return A.domain.dist;
  // A and its elements are destroyed here
}


proc test() {
  var v1 = badReturnBorrowedArrayDom();
  var v2 = badReturnBorrowedArrayDomDist();
  writeln(v1);
  writeln(v2);
}

test();
}
