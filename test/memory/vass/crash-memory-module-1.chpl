use Memory;
writeln("starting...");

// make sure memoryUsed() is invoked, but ignore the result
config const forceit = 0;
const m = memoryUsed():int;
writeln(m * forceit);
