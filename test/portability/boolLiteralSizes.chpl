use SysCTypes;

extern proc sizeof(e): size_t;

writeln("sizes of bool literals");
writeln(sizeof(true:bool(8)));
writeln(sizeof(true:bool(16)));
writeln(sizeof(true:bool(32)));
writeln(sizeof(true:bool(64)));
// writeln(sizeof(true); Non-portable
writeln();
