// Expected: range(uint(64), bounded, false);
writeln(typeToString((1:int(8)..100:uint(64)).type));

// Expected: range(uint(64), bounded, false);
writeln(typeToString((1:int(16)..100:uint(64)).type));

// Expected: range(uint(64), bounded, false);
writeln(typeToString((1:uint(64)..100:int(8)).type));
writeln(typeToString((1:uint(64)..100:int(16)).type));
