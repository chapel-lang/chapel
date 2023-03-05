var t: bool = true;
var f: bool = false;

operator ~(a: bool) do return !a;

writeln("~true = ", ~t);
writeln("~false = ", ~f);
