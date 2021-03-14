// typeMath.chpl
// Test out various type combinations for ranges.

writeln((1:int..).type:string);
writeln((..10:int).type:string);
writeln((1:int..10).type:string);
writeln((1..10:int).type:string);
writeln((1:int..10:int).type:string);

writeln("-----");

writeln((1:uint..).type:string);
writeln((..10:uint).type:string);
writeln((1:uint..10).type:string);
writeln((1..10:uint).type:string);
writeln((1:uint..10:uint).type:string);

writeln("-----");

writeln((1:int(8)..).type:string);
writeln((..10:int(8)).type:string);
writeln((1:int(8)..10).type:string);
writeln((1..10:int(8)).type:string);
writeln((1:int(8)..10:int(8)).type:string);

writeln((1:uint(8)..).type:string);
writeln((..10:uint(8)).type:string);
writeln((1:uint(8)..10).type:string);
writeln((1..10:uint(8)).type:string);
writeln((1:uint(8)..10:uint(8)).type:string);

writeln("-----");

writeln((1:int(16)..).type:string);
writeln((..10:int(16)).type:string);
writeln((1:int(16)..10).type:string);
writeln((1..10:int(16)).type:string);
writeln((1:int(16)..10:int(16)).type:string);

writeln("-----");

writeln((1:uint(16)..).type:string);
writeln((..10:uint(16)).type:string);
writeln((1:uint(16)..10).type:string);
writeln((1..10:uint(16)).type:string);
writeln((1:uint(16)..10:uint(16)).type:string);

writeln("-----");

writeln((1:int(32)..).type:string);
writeln((..10:int(32)).type:string);
writeln((1:int(32)..10).type:string);
writeln((1..10:int(32)).type:string);
writeln((1:int(32)..10:int(32)).type:string);

writeln("-----");

writeln((1:uint(32)..).type:string);
writeln((..10:uint(32)).type:string);
writeln((1:uint(32)..10).type:string);
writeln((1..10:uint(32)).type:string);
writeln((1:uint(32)..10:uint(32)).type:string);

writeln("-----");

writeln((1:int(64)..).type:string);
writeln((..10:int(64)).type:string);
writeln((1:int(64)..10).type:string);
writeln((1..10:int(64)).type:string);
writeln((1:int(64)..10:int(64)).type:string);

writeln("-----");

writeln((1:uint(64)..).type:string);
writeln((..10:uint(64)).type:string);
writeln((1:uint(64)..10).type:string);
writeln((1..10:uint(64)).type:string);
writeln((1:uint(64)..10:uint(64)).type:string);

writeln("-----");

writeln((1:int..100:int).type:string);
writeln((1:int..100:int(8)).type:string);
writeln((1:int..100:int(16)).type:string);
writeln((1:int..100:int(32)).type:string);
writeln((1:int..100:int(64)).type:string);
writeln((1:int..100:uint).type:string);
writeln((1:int..100:uint(8)).type:string);
writeln((1:int..100:uint(16)).type:string);
writeln((1:int..100:uint(32)).type:string);
writeln((1:int..100:uint(64)).type:string);
writeln((1:int(8)..100:int).type:string);
writeln((1:int(8)..100:int(8)).type:string);
writeln((1:int(8)..100:int(16)).type:string);
writeln((1:int(8)..100:int(32)).type:string);
writeln((1:int(8)..100:int(64)).type:string);
writeln((1:int(8)..100:uint(8)).type:string);
writeln((1:int(8)..100:uint(16)).type:string);
writeln((1:int(8)..100:uint(32)).type:string);
writeln((1:int(16)..100:int).type:string);
writeln((1:int(16)..100:int(8)).type:string);
writeln((1:int(16)..100:int(16)).type:string);
writeln((1:int(16)..100:int(32)).type:string);
writeln((1:int(16)..100:int(64)).type:string);
writeln((1:int(16)..100:uint(8)).type:string);
writeln((1:int(16)..100:uint(16)).type:string);
writeln((1:int(16)..100:uint(32)).type:string);
writeln((1:int(32)..100:int).type:string);
writeln((1:int(32)..100:int(8)).type:string);
writeln((1:int(32)..100:int(16)).type:string);
writeln((1:int(32)..100:int(32)).type:string);
writeln((1:int(32)..100:int(64)).type:string);
writeln((1:int(32)..100:uint(8)).type:string);
writeln((1:int(32)..100:uint(16)).type:string);
writeln((1:int(32)..100:uint(32)).type:string);
writeln((1:int(64)..100:int).type:string);
writeln((1:int(64)..100:int(8)).type:string);
writeln((1:int(64)..100:int(16)).type:string);
writeln((1:int(64)..100:int(32)).type:string);
writeln((1:int(64)..100:int(64)).type:string);
writeln((1:int(64)..100:uint).type:string);
writeln((1:int(64)..100:uint(8)).type:string);
writeln((1:int(64)..100:uint(16)).type:string);
writeln((1:int(64)..100:uint(32)).type:string);
writeln((1:int(64)..100:uint(64)).type:string);

writeln("-----");

writeln((1:uint..100:int).type:string);
writeln((1:uint..100:int(64)).type:string);
writeln((1:uint..100:uint).type:string);
writeln((1:uint..100:uint(8)).type:string);
writeln((1:uint..100:uint(16)).type:string);
writeln((1:uint..100:uint(32)).type:string);
writeln((1:uint..100:uint(64)).type:string);
writeln((1:uint(8)..100:int).type:string);
writeln((1:uint(8)..100:int(8)).type:string);
writeln((1:uint(8)..100:int(16)).type:string);
writeln((1:uint(8)..100:int(32)).type:string);
writeln((1:uint(8)..100:int(64)).type:string);
writeln((1:uint(8)..100:uint).type:string);
writeln((1:uint(8)..100:uint(8)).type:string);
writeln((1:uint(8)..100:uint(16)).type:string);
writeln((1:uint(8)..100:uint(32)).type:string);
writeln((1:uint(8)..100:uint(64)).type:string);
writeln((1:uint(16)..100:int).type:string);
writeln((1:uint(16)..100:int(8)).type:string);
writeln((1:uint(16)..100:int(16)).type:string);
writeln((1:uint(16)..100:int(32)).type:string);
writeln((1:uint(16)..100:int(64)).type:string);
writeln((1:uint(16)..100:uint).type:string);
writeln((1:uint(16)..100:uint(8)).type:string);
writeln((1:uint(16)..100:uint(16)).type:string);
writeln((1:uint(16)..100:uint(32)).type:string);
writeln((1:uint(16)..100:uint(64)).type:string);
writeln((1:uint(32)..100:int).type:string);
writeln((1:uint(32)..100:int(8)).type:string);
writeln((1:uint(32)..100:int(16)).type:string);
writeln((1:uint(32)..100:int(32)).type:string);
writeln((1:uint(32)..100:int(64)).type:string);
writeln((1:uint(32)..100:uint).type:string);
writeln((1:uint(32)..100:uint(8)).type:string);
writeln((1:uint(32)..100:uint(16)).type:string);
writeln((1:uint(32)..100:uint(32)).type:string);
writeln((1:uint(32)..100:uint(64)).type:string);
writeln((1:uint(64)..100:int).type:string);
writeln((1:uint(64)..100:int(64)).type:string);
writeln((1:uint(64)..100:uint).type:string);
writeln((1:uint(64)..100:uint(8)).type:string);
writeln((1:uint(64)..100:uint(16)).type:string);
writeln((1:uint(64)..100:uint(32)).type:string);
writeln((1:uint(64)..100:uint(64)).type:string);
