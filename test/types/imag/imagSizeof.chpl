use SysCTypes;

extern proc sizeof(e): ssize_t;

writeln(sizeof(1.0i));
writeln(sizeof(1.2i));
writeln(sizeof(2.0i));
writeln(sizeof(3i));
writeln(sizeof(3.0i));
writeln(sizeof(4.0i));
writeln(sizeof(4i));
