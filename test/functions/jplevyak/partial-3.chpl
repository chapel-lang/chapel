function f(x, y, z) { writeln(x + y + z); }
var g = => f(1);
var h = => g(2);
h(3);
