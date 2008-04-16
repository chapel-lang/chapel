_extern def printMemStat(lineno=-1, filename="");

var m = 5;
var n = 2;

m *= n;
writeln("m = ", m);

printMemStat();