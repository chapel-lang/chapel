// Simple tests of recently-introduced syntactic forms.

var ARR1 = [111, 222, 333, 444];
var ARR2 = ["a", "b", "c", "d"];
var INT = 777;

writeln("forall zip(ARR1,ARR2)");
forall zip(ARR1,ARR2) do writeln(INT);

writeln("forall zip(ARR1,ARR2) with");
forall zip(ARR1,ARR2) with (ref INT) do writeln(INT);

writeln("[ zip(ARR1,ARR2) ]");
[ zip(ARR1,ARR2) ] writeln(INT);

writeln("[ zip(ARR1,ARR2) with ]");
[ zip(ARR1,ARR2) with (ref INT) ] writeln(INT);

writeln("done");
