var A: [1..4 by -1, 1..4] int;        // neg-stride
writeln(A); writeln();

var C: [1..4 by -1, 1..4 by -1] int;  // neg-stride
writeln(C); writeln();

var X: [1..4 by -2, 1..4] int;        // neg-stride
writeln(X); writeln();

// this one is OK:
var B: [1..4, 1..4 by -1] int;        // neg-stride
writeln(B); writeln();
