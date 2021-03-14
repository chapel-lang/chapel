config const n = 11;

var A: [n..#n] int = n;
var B = 2*A;

if (+ reduce A-B) == 11 then writeln("hi");
