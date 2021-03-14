proc printit(name, param arg) {
  writeln(name, " ", arg.type:string, " ", arg, " ", arg:string);
}

printit("0.0", 0.0);
printit("-0.0", -0.0);
printit("0.0-0.0", 0.0-0.0);
printit("0.0+0.0i", 0.0+0.0i);
printit("0.0-0.0i", 0.0-0.0i);
printit("-(0.0+0.0i)", -(0.0+0.0i));
printit("1.0+0.0i", 1.0+0.0i);
printit("1.0-0.0i", 1.0-0.0i);
printit("-(1.0+0.0i)", -(1.0+0.0i));
