proc main() {
  var r = 1..10;
  var len = r.length;
  var str = len:string;
  __primitive("chpl_error", str);
}

