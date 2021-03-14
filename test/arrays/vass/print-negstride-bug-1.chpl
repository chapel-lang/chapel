var A: [1..4 by -1, 1..4] int;
writeln(A); writeln();

var C: [1..4 by -1, 1..4 by -1] int;
writeln(C); writeln();

var X: [1..4 by -2, 1..4] int;
writeln(X); writeln();

// this one is OK:
var B: [1..4, 1..4 by -1] int;
writeln(B); writeln();
