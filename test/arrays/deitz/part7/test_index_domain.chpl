config var n : int = 8;
var d = {1..n, 1..n};
var innerD = {2..n-1, 2..n-1};

var a : [d] int;

a = 0;

writeln(a);

a(innerD) = 1;

writeln(a);
