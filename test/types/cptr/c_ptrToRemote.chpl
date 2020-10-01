use CPtr;

var A: [1..10] real;

on Locales[numLocales-1] do
  var aptr = c_ptrTo(A);
