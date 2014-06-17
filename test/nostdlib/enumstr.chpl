enum F { a=1, b, c, d=-3, e, f };

proc tostring(x:?t) {
  return x:string;
}

proc main() {
  var str = tostring(F.d);
  var len = str.length;
  var ret = str + (len:string);
  __primitive("chpl_error", ret);
}

