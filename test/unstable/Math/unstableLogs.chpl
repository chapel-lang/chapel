use Math;

// log2
var log2Int = log2(4);
writeln(log2Int.type: string);
writeln(log2Int);
var log2UInt = log2(4: uint);
writeln(log2UInt.type: string);
writeln(log2UInt);

// ln
var lnInt = ln(e: int);
writeln(lnInt.type: string);
writeln(lnInt);
var lnUInt = ln(e: uint);
writeln(lnUInt.type: string);
writeln(lnUInt);

writeln("-- log --");
// log
var logInt = log(e: int(16));
writeln(logInt.type: string);
writeln(logInt);
var logUInt = log(e: uint(8));
writeln(logUInt.type: string);
writeln(logUInt);

writeln("-- log10 --");
var log10Int = log10(100);
writeln(log10Int.type: string);
writeln(log10Int);
var log10UInt = log10(1000: uint);
writeln(log10UInt.type: string);
writeln(log10UInt);

writeln("-- log1p --");
var log1pInt = log1p(e: int(16));
writeln(log1pInt.type: string);
writeln(log1pInt);
var log1pUInt = log1p(e: uint(8));
writeln(log1pUInt.type: string);
writeln(log1pUInt);
