use SysCTypes;

extern proc sizeof(e): size_t;

writeln("sizes of real literals");
writeln(sizeof(1.0:real(32)));
writeln(sizeof(1.0:real(64)));
writeln(sizeof(1.0));
writeln();

writeln("sizes of imaginary literals");
writeln(sizeof(1.0i:imag(32)));
writeln(sizeof(1.0i:imag(64)));
writeln(sizeof(1.0i));
writeln();

writeln("sizes of complex literals");
writeln(sizeof((1.0 + 1.0i):complex(64)));
writeln(sizeof((1.0 + 1.0i):complex(128)));
writeln(sizeof((1.0 + 1.0i)));
writeln();
