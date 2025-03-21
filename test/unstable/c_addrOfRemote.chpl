use CTypes;

var A: [1..10] real;

on Locales[numLocales-1] {
  var aptr = c_addrOf(A);
  var aconstptr = c_addrOfConst(A);
}
