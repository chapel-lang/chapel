var t: bool = true;
var f: bool = false;

operator bool.~(a: bool) return !a;

writeln("~true = ", ~t);
writeln("~false = ", ~f);
