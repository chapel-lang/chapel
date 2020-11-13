use ConcurrentMap;
use utilFunctions;

var m1 = new ConcurrentMap(string, int);
var m2 = new ConcurrentMap(string, int);

forall i in 1..20 with (ref m1) do
  m1.add(intToEnglish(i), i);

forall i in 19..40 with (ref m2) do
  m2.add(intToEnglish(i), -i);

m1.extend(m2);
var A = m1.values();
writeln(A.sorted());
