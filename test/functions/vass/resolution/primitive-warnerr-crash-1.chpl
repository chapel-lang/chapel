// the issue is the same with the "error" primitive
proc co1(param test: bool, param x:string ...?n) {
  __primitive("warning", (...x));
}
proc co2(param test: bool, param x:string ...?n) {
  __primitive("warning", "this is a warning");
}
co1(false, "a", "a", "a", "a", "a");
co2(false, "a", "a", "a", "a", "a");
