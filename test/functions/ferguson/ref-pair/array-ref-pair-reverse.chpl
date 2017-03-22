config param error = false;

var global:[1..3] int;

// It is typical for a ref-pair method to return
// a ref if the appropriate argument is a ref.
// This test checks the opposite scenario.

proc global_getter(arg) ref {
  writeln("GLOBAL REF");
  readit(arg); // const on arg
  return global; // returns ref to global
}

proc global_getter(arg) const ref {
  writeln("GLOBAL CONST REF");
  setit(arg); // ref on arg
  return global; // returns const ref to global
}

proc arg_getter(arg) ref {
  writeln("ARG REF");
  return arg;
}

proc arg_getter(arg) const ref {
  writeln("ARG CONST REF");
  return arg;
}

proc setit(x) {
  x[1] = 1;
}

proc readit(x) {
  writeln(x[1]);
}

var arg:[1..3] int;
writeln("setit");
setit(global_getter(arg_getter(arg)));
writeln("readit");
readit(global_getter(arg_getter(arg)));

const constArg:[1..3] int;
writeln("setit");
setit(global_getter(constArg));
if error then
  readit(global_getter(constArg));
