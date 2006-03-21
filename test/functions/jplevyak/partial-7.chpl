fun f(a, b, c, d, e) { writeln(a + b + c + d + e); }
var g = => f(1);
var h = => g(2);
var i = => h(3);
var j = => i(4);
j(5);
