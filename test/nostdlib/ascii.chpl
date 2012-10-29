proc main() {
  var s = "test";
  var c = __primitive("ascii", s);
  var str = c:string;
  __primitive("chpl_error", str);
}

