use SysCTypes;

extern proc sizeof(e): size_t;

/* Testing that the size reported is correct */
writeln(sizeof(true:bool(8)));
writeln(sizeof(false:bool(16)));
writeln(sizeof(true:bool(32)));
writeln(sizeof(false:bool(64)));

writeln();
/* Testing the that output shown is still the correct one */
writeln(true:bool(8));
writeln(false:bool(16));
writeln(true:bool(32));
writeln(false:bool(64));
