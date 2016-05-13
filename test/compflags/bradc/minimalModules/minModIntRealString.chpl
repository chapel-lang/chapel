// Showcase what works with --minimal-modules and how to add more.

extern proc printf(f:c_string, x...);
proc +(a, b) return __primitive("+", a, b);

var i: int;
var r: real;
var str = "Hello";
var cs  = c"world";

i = 5;
r = 6.6;
i = i + 7;

printf("%ld %g  %s, %s!\n", i, r, str: c_string, cs);
