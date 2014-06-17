config const n = 97;

var D0: domain(1) = {3..n};
on Locales[numLocales-1] do
  writeln(D0);

var D1: domain(1);
D1 = {3..n};
on Locales[numLocales-1] do
  writeln(D1);

var D2 = {3..n};
on Locales[numLocales-1] do
  writeln(D2);

var D3 = f3();
on Locales[numLocales-1] do
  writeln(D3);

var D4 = f4();
on Locales[numLocales-1] do
  writeln(D4);

proc f3() {
  return D0;
}

proc f4() {
  var D = {3..n};
  return D;
}

proc f5() {
  return {3..n};
}
