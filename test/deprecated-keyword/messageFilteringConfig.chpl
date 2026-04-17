@deprecated
config const x000 = 1;
@deprecated
config param x001 = 2;
@deprecated("I am deprecated")
config const x002 = 3;
@deprecated("I am deprecated")
config param x003 = 4;
@deprecated("I am deprecated: :mod:`spam`")
config const x004 = 5;
@deprecated("I am deprecated: :proc:`spam`")
config param x005 = 6;

writeln(x000);
writeln(x001);
writeln(x002);
writeln(x003);
writeln(x004);
writeln(x005);
