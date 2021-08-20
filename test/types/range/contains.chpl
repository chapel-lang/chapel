var r: range(stridable=false);
var r2: range(stridable=false);


writeln(r.contains(r2));
r = 1..10;
writeln(r.contains(r2));
writeln(r2.contains(r));

