var s1 : seq of int = _seq(1, 2, 3, 4);
var s2 : seq of int = _seq(5, 6, 7, 8);

writeln(s1);
writeln(s2);

var ss : seq of seq of int = _seq(s1, s2);

writeln(ss);

forall s in ss do
  writeln(s);

forall s in ss do
  forall i in s do
    writeln(i);
