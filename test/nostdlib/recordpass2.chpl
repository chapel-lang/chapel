record R {
  var x:int;
}

proc rfun():R {
  var ret:R;
  ret.x = 17;
  return ret;
}

proc main() {
  var r:R = rfun();
  var str = r:string;
  __primitive("chpl_error", str);
}

