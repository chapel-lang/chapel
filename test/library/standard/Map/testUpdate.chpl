use Map;
use utilFunctions;

var m1 = new map(string, int, true);
var m2 = new map(string, int, true);

forall i in 1..20 with (ref m1) do
  m1[intToEnglish(i)] = i;
forall i in 19..40 with (ref m2) do
  m2[intToEnglish(i)] = -i;

m1.update(m2);
var A = m1.values();
writeln(A.sorted());
