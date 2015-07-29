for i in 1..4**3 {
  writeln("i = ", i, "; log4(i) = ", log4(i));
}

proc log4(in x) {
  // Assumes passed val >= 1, of type int or uint
  var result = -1;
  while (x != 0) {
    x >>= 2; // base 4 in log 2
    result += 1;
  }
  return result;
}
