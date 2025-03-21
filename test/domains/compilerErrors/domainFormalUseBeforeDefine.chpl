proc domainArgsInRange(matrix:[0..#D1][0..#D2] real, D1 : int, D2 : int) {
  // should be an error to use D1 and D2 before defined
}

proc domainArg(ref A:[dom1] real, ref B:[dom1] real, dom1:domain(1)) {
  // should be an error to use dom1 before defined
}
