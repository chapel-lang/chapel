use SysCTypes;

extern proc sizeof(e): size_t;

writeln("sizes of signed integer literals");
writeln(sizeof(1:int(8)));
writeln(sizeof(1:int(16)));
writeln(sizeof(1:int(32)));
writeln(sizeof(1:int(64)));
writeln(sizeof(1));
writeln();

writeln("sizes of unsigned integer literals");
writeln(sizeof(1:uint(8)));
writeln(sizeof(1:uint(16)));
writeln(sizeof(1:uint(32)));
writeln(sizeof(1:uint(64)));
writeln(sizeof(1));
writeln();
