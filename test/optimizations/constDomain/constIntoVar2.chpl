var A:[1..10] int;
writeln("A.domain definedConst=", A.domain._value.definedConst);

proc returnADomainCopy() {
  return A.domain; // copies to a non "view"
}

proc test1() {
  const D = returnADomainCopy();
  writeln("test1 D definedConst=", D._value.definedConst);
}
test1();

proc test2() {
  var D = returnADomainCopy();
  writeln("test2 D definedConst=", D._value.definedConst);
  // Could now do D = {1..2} e.g. ; so D's definedConst must not be true
}
test2();
