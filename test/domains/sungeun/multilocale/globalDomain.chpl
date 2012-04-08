config const n = 97;

var D0: domain(1) = [3..n];
on Locales[numLocales-1] do
  writeln(D0);

var D1: domain(1);
D1 = [3..n];
on Locales[numLocales-1] do
  writeln(D1);

var D2 = [3..n];
on Locales[numLocales-1] do
  writeln(D2);

