/*
  Function formals should be used in the function body.
*/

// This function has an unused formal which violates the rule.
proc badProc(x: int, y: int) {
  writeln(x);
}

// This function uses all formals which is correct.
proc goodProc(x: int, y: int) {
  writeln(x, y);
}
