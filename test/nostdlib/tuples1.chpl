proc main() {
  var t = (1,2,3,4);
  var str = t:string;
  __primitive("chpl_error", str);
}

