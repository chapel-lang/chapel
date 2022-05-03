// Expect this to behave the same as return-domain.chpl

proc returnType() type {
  var A:[1..10] int;

  return A.type;
}

var B:returnType();
writeln(B);
