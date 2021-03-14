config var er: int = 1;
config var hz: int = 2;
config var ok: int = 3;

writeln("*****************************************************");
writeln("Given a hash table size of 101, these three variables");
writeln("hash to the same location.");
writeln("*****************************************************");

writeln("er is initialized at compile time to 1");
writeln("er at runtime is ", er);

writeln("hz is initialized at compile time to 2");
writeln("hz at runtime is ", hz);

writeln("ok is initialized at compile time to 3");
writeln("ok at runtime is ", ok);
