use Communication;
use CTypes;

var A = [i in 1..10] i;
var APtr = c_ptrTo(A); // can't do this remotely

on Locales[numLocales-1] {
  var B: [1..10] int;

  get(c_ptrTo(B), APtr, A.locale.id, A.size*numBytes(A.eltType));

  writeln(B);

  B += 1;

  put(APtr, c_ptrTo(B), A.locale.id, B.size*numBytes(B.eltType));
}

writeln(A);
