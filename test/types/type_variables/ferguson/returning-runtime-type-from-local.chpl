proc getTypeFromLocal() type {
  var A:[1..100] int;
  return A.type;
}

proc test() {
  var B: getTypeFromLocal();
  writeln(B.domain);
  B[1] = 1;
  B[99] = 99;
  writeln(B);
}

test();
