class bar : reduction {}

class reduction {} -- override built-in reduction class?

class foo : reduction {} -- this is not a real reduction...

var D: domain = (1..m, 1..n);
var A: [D] float;
var a: float;

a = max reduce A;
a = bar reduce A;
a = foo reduce A;
