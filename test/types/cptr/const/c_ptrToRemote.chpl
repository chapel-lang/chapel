use CTypes;

var A: [1..10] real;

on Locales[numLocales-1] do
  var aptr_const = c_ptrToConst(A);
