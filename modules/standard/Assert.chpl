proc assert(test: bool) {
  if !test then
    __primitive("chpl_error", "assert failed");
}
  
  
proc assert(test: bool, args ...?numArgs) {
  if !test {
    var tmpstring: c_string;
    tmpstring.write((...args));
    __primitive("chpl_error", "assert failed - " + tmpstring);
  }
}
