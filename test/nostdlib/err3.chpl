proc main() {
  var err = EFORMAT;
  var x = 0;
  if err {
    x = 1;
  }
  var str = x:string;
  __primitive("chpl_error", str);
}

