var x : int;
x = 1;
proc f() ref : int { return x; }
f() = 2;
writeln(x);
