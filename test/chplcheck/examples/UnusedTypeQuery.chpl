/*
  Type queries should be used in the function body.

  A type query such as ``?t`` names the type of a formal so the body can refer
  to it.  If the body never mentions the name, the query only obscures the
  signature, so either use it or remove it.
*/

// This function has an unused type query, which violates the rule.
proc badProc(ref a: ?t) {
  a = 1;
}

// This function uses its type query, which is correct.
proc goodProc(ref a: ?t) {
  writeln(a, " has type ", t: string);
}

// Removing the type query is also correct.
proc alsoFine(ref a) {
  a = 1;
}
