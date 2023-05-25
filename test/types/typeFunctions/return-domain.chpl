// This is a version of return-runtime-type.chpl
// that uses a domain instead of a runtime type

proc returnType() {
  var A:[1..10] int;

  return A.domain;
}

proc test() {
  var B:[returnType()] int;
  writeln(B);
}

test();
