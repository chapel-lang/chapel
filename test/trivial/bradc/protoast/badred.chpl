class bar : reduction {}

class reduction {} -- override built-in reduction class?

class foo : reduction {} -- this is not a real reduction...

var D: domain = [1..m, 1..n];
var A: [D] float;
var a: float;

a = reduce by max A;
a = max A;
a = reduce by bar A;
a = bar A;
a = reduce by foo A;
a = foo A;
