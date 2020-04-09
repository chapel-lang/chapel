proc paramprint(param arg) {
  compilerWarning(arg.type:string, " ", arg:string);
}

param x:real(32) = 1.0:real(32);
writeln("x=", x);
paramprint(x);
param y:real(32) = 2.0;
writeln("y=", y);
paramprint(y);
param z = 3.0:real(32);
writeln("z=", z);
paramprint(z);
