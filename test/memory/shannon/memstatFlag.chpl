use MemDiagnostics;

var m = 5;
var n = 2;

printMemAllocStats();
m *= n;
writeln("m = ", m);

