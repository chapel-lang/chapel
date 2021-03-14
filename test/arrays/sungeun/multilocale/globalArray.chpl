config const n = 7;
const D = {3..n};

var A0: [D] real = [i in D] i:real;
on Locales[numLocales-1] do
  writeln(A0);

var A1: [D] real;
A1 = [i in D] i:real;
on Locales[numLocales-1] do
  writeln(A1);

var A2 = [i in D] i:real;
on Locales[numLocales-1] do
  writeln(A2);

var A3 = f3();
on Locales[numLocales-1] do
  writeln(A3);

var A4 = f4();
on Locales[numLocales-1] do
  writeln(A4);

proc f3() {
  return A0;
}

proc f4() {
  var A = [i in D] i:real;
  return A;
}
