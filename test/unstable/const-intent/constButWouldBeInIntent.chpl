var globalInt: int;

proc takeRec(const r: int) {
  writeln(r);
  writeln(globalInt);
  // This should not trigger the warning, ints are passed by `const in`
  globalInt = 3;
  writeln(r);
  writeln(globalInt);
}

takeRec(globalInt);
