/*
  Type queries should be used in the function body.
*/

// This function has an unused type query which violates the rule.
proc badProc(ref a: ?t) {
  a = 1;
}

// This function uses its type query which is correct.
proc goodProc(ref a: ?t) {
  writeln(a, " has type ", t: string);
}
