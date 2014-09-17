// This test highlights the evaluation order of the actuals.

proc showargs(arg1: int, arg2: int) { writeln("showargs", (arg1, arg2)); }

var myVar = 111;

proc updateVar() {
  const result = myVar;
  myVar += 111;
  writeln("in updateVar(): myVar ", result, " -> ", myVar);
  return result;
}

// Evaluate the actuals in the order of their occurrence => showargs(111,111)
showargs(myVar, updateVar());

showargs(updateVar(), myVar);
showargs(arg2 = myVar, arg1 = updateVar());
showargs(arg2 = updateVar(), arg1 = myVar);
writeln();

proc msg(message: string, arg: int) {
  writeln(message, arg);
  return arg;
}

showargs(msg("first arg ", 666),
         msg("second arg ", 777));

showargs(arg2 = msg("arg2/first actual = ", 777),
         arg1 = msg("arg1/second actual = ", 666));
