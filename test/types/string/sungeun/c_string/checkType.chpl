proc checkType(type t, param d=2) {
  checkType(string, t, d+1);
}

proc checkType(type te, type ta, param d=2) {
  if te != ta then
    compilerError("Expected type "+typeToString(te)+", got "+typeToString(ta), d);
}

