iter i { yield 1..3; }
proc f return 1..3;
var x = {i}; // what is x?
var y = {f}; // what is y?
writeln(x);
writeln(y);

