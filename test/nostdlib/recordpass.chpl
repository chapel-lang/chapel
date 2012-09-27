record R {
  var x:int;
}

proc rfun(r:R) {
  return r.x;
}

proc main() {
  var r:R;
  r.x = 17;
  var x = rfun(r);
  var str = x:string;
  __primitive("chpl_error", str);
}

