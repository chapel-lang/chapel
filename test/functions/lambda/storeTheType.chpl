proc foo() {
  writeln("in foo");
}

// The following compiles and runs
//var tempfcf = foo;
//type argType = tempfcf.type;
type argType = foo.type;
writeln(argType: string);

