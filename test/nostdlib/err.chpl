proc main() {
  var err:syserr = ENOERR;
  var x = 0;
  if !err {
    x = 1;
  }
  var str = x:string;
  __primitive("chpl_error", str);
}

