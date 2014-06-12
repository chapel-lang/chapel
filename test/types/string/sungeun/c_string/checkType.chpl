proc checkType(type t, param d=3) {
  checkType(string, t, d+1);
}

proc checkType(type te, type ta, param d=3) {
  if te != ta then
    compilerError("Expected type "+typeToString(te)+", got "+typeToString(ta), d);
}

