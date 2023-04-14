var r: simpleRange; /*autofix*/
var r2: simpleRange; /*autofix*/


writeln(r.contains(r2));
r = 1..10;
writeln(r.contains(r2));
writeln(r2.contains(r));

