var f = proc(n: int): int { return 1; } ;
var x = f(1);
writeln(x);

var g = proc(n: int): int do { return 2; } ;
var y = g(1);
writeln(y);

var h = proc(n: int): int do return 3; ;
var z = h(1);
writeln(z);
