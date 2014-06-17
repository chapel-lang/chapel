var x : int;
x = 1;
proc f() var : int { return x; }
f() = 2;
writeln(x);
